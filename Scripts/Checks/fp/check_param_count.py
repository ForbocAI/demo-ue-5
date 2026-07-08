#!/usr/bin/env python3
"""UE runtime parameter-count compliance scanner.

Mirrors Therapy 12's parameter discipline
(therapy-12/SCRIPTS/check_param_count.py) for this C++ runtime: keep authored
function DEFINITIONS at two data parameters or fewer and drive over-arity
functions toward functional composition. Each over-limit definition is bucketed
by WHY it carries the extra parameters; only the genuinely reducible
("actionable") bucket fails the guard.

Buckets:
    actionable   - foldable via currying / a payload struct.
    fn-arg       - a parameter is already a function/callable ("supply
                   functions as arguments"); leave at two.
    mut-sink     - the first parameter is a non-const &/* output or engine
                   target written each call. A `const` reference/pointer is
                   DATA, not a sink.
    reducer      - a Reduce*(state, action) function whose first parameter is
                   the by-value state accumulator; leave at two.
    ue-callback  - UE reflection/lifecycle callback (BeginPlay, Tick, ...) whose
                   signature is owned by the engine boundary.

The reduction-by-currying technique (capture the threaded constant in a factory
that returns a one-argument function) and the payload-struct boundary are the
ue_fp cookbook patterns; see Core/ue_fp.hpp.
"""

from __future__ import annotations

import argparse
import pathlib
import re
import sys
from collections import Counter


PROJECT_ROOT = pathlib.Path(__file__).resolve().parents[2]
SOURCE_ROOT = PROJECT_ROOT / "Source"
MAX_DATA_PARAMS = 2

FN_RE = re.compile(
    r"(?:^|[;\n{}])\s*(?:[\w:<>&*\s]+\s+)?([A-Za-z_][A-Za-z0-9_:~]*)\s*\(",
    re.MULTILINE,
)
CONTROL = {"if", "for", "while", "switch", "return", "sizeof", "TEXT"}
UE_CALLBACKS = {
    "BeginPlay",
    "Tick",
    "TickComponent",
    "SetupInputComponent",
    "InitGame",
    "NativeConstruct",
    "HandleInteractionBegin",
    "HandleInteractionEnd",
}

GUIDANCE = (
    "Keep authored functions at two data parameters or fewer. A leading CONST "
    "reference/pointer is DATA -- do not widen a signature by "
    "threading more const inputs. Reduce an actionable function with the ue_fp "
    "cookbook: (1) group related inputs into one payload/request struct at a "
    "real domain boundary; (2) curry -- capture the stable inputs in a factory "
    "that returns a unary function over the varying input; or (3) supply "
    "behavior as a function argument (func::compose/pipe/converge2, folds, "
    "catalogs, Dispatcher). Non-actionable classifications are still reported: "
    "fn-arg (a parameter is already a function/callable), mut-sink (a non-const "
    "&/* output or engine target), reducer (Reduce* with a by-value state), and "
    "UE reflection/lifecycle callbacks."
)

# A parameter whose type is a function/callable (the endorsed "supply functions
# as arguments" pattern) rather than plain data.
FN_TYPE_RE = re.compile(
    r"std::function\s*<|\bT(?:Unique)?Function(?:Ref)?\s*<|\(\s*\*"
)


def split_top_level(text: str):
    depth = 0
    start = 0
    pairs = {"(": ")", "<": ">", "[": "]", "{": "}"}
    closers = set(pairs.values())
    for index, char in enumerate(text):
        if char in pairs:
            depth += 1
        elif char in closers:
            depth -= 1
        elif char == "," and depth == 0:
            yield text[start:index]
            start = index + 1
    yield text[start:]


def matched_paren_body(text: str, open_index: int) -> str | None:
    depth = 1
    index = open_index + 1
    while index < len(text) and depth > 0:
        char = text[index]
        if char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
        index += 1
    if depth != 0:
        return None
    return text[open_index + 1 : index - 1]


def strip_receiver(name: str) -> str:
    return name.rsplit("::", 1)[-1].lstrip("~")


def data_segments(param_text: str) -> list[str]:
    segments = [segment.strip() for segment in split_top_level(param_text)]
    return [segment for segment in segments if segment and segment != "void"]


def is_mut_sink(segment: str) -> bool:
    # A mutable output/engine target: a non-const reference or pointer. A
    # `const &`/`const *` parameter is DATA, not a sink.
    return ("&" in segment or "*" in segment) and "const" not in segment


def classify(name: str, segments: list[str], tail: str) -> str:
    # Mirrors Therapy 12's classify_function: only genuinely reducible
    # ("actionable") functions fail the guard; the other buckets are
    # irreducible by design and stay at two parameters.
    simple_name = strip_receiver(name)
    if simple_name in UE_CALLBACKS or re.search(r"\boverride\b", tail):
        return "ue-callback"
    if any(FN_TYPE_RE.search(segment) for segment in segments):
        return "fn-arg"
    if segments and is_mut_sink(segments[0]):
        return "mut-sink"
    if (
        simple_name.startswith("Reduce")
        and segments
        and "&" not in segments[0]
        and "*" not in segments[0]
    ):
        return "reducer"
    return "actionable"


def scan_file(path: pathlib.Path):
    text = path.read_text(encoding="utf-8", errors="replace")
    for match in FN_RE.finditer(text):
        name = match.group(1)
        simple_name = strip_receiver(name)
        if simple_name in CONTROL:
            continue
        open_index = match.end() - 1
        body = matched_paren_body(text, open_index)
        if body is None:
            continue
        # Position the tail just past the parameter list's ")" and require a
        # definition body "{" (optionally after const/override/final/noexcept or
        # a trailing return type). Ending in ";" is a declaration or call site,
        # not an authored definition, so it is not scanned.
        close_index = open_index + len(body) + 2
        tail = text[close_index : close_index + 80]
        if not re.match(
            r"\s*(?:const\s*)?(?:override\s*)?(?:final\s*)?(?:noexcept\s*)?"
            r"(?:->[^{;]+)?\{",
            tail,
        ):
            continue
        segments = data_segments(body)
        yield name, len(segments), text[: match.start()].count("\n") + 1, classify(name, segments, tail)


def gather(root: pathlib.Path):
    violations = []
    for path in sorted(root.rglob("*")):
        if path.suffix not in {".h", ".hpp", ".cpp"}:
            continue
        for name, params, line, bucket in scan_file(path):
            if params > MAX_DATA_PARAMS:
                violations.append((path, line, name, params, bucket))
    return violations


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog="This guard always scans the project Source tree; path and threshold arguments are intentionally unsupported.",
    )
    parser.parse_args()

    violations = gather(SOURCE_ROOT)
    actionable = [row for row in violations if row[4] == "actionable"]
    counts = Counter(row[4] for row in violations)

    print(f"Functions over {MAX_DATA_PARAMS} data params: {len(violations)}")
    print(f"  actionable (foldable):       {counts['actionable']}")
    print(f"  fn-arg (functions-as-args):  {counts['fn-arg']}")
    print(f"  mut-sink (&/* target):       {counts['mut-sink']}")
    print(f"  reducer (state + action):    {counts['reducer']}")
    print(f"  ue-callback (engine-fixed):  {counts['ue-callback']}")

    for path, line, name, params, bucket in actionable:
        print(f"{path}:{line}: {params} params: {name}")

    if actionable:
        print("")
        print(GUIDANCE)

    return 1 if actionable else 0


if __name__ == "__main__":
    sys.exit(main())
