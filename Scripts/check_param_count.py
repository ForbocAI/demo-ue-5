#!/usr/bin/env python3
"""UE demo parameter-count compliance scanner.

Mirrors the Therapy 12 parameter discipline for this C++ demo: keep ordinary
functions at two data parameters or fewer, group repeated scalar clusters into
payload structs, and leave UE reflection/lifecycle callbacks in a separate
engine-callback bucket.
"""

from __future__ import annotations

import argparse
import pathlib
import re
import sys
from collections import Counter


FN_RE = re.compile(
    r"(?:^|[;\n{}])\s*(?:[\w:<>&*\s]+\s+)?([A-Za-z_][A-Za-z0-9_:~]*)\s*\(",
    re.MULTILINE,
)
CONTROL = {"if", "for", "while", "switch", "return", "sizeof", "TEXT"}
UE_CALLBACKS = {
    "BeginPlay",
    "Tick",
    "SetupInputComponent",
    "NativeConstruct",
    "HandleInteractionBegin",
    "HandleInteractionEnd",
}


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


def classify(name: str, segments: list[str]) -> str:
    simple_name = strip_receiver(name)
    if simple_name in UE_CALLBACKS:
        return "ue-callback"
    if segments and any(token in segments[0] for token in ("&", "*")):
        return "mut-or-engine-sink"
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
        close_index = open_index + len(body) + 1
        tail = text[close_index : close_index + 80]
        if not re.match(r"\s*(?:const\s*)?(?:override\s*)?(?:final\s*)?(?:noexcept\s*)?[{;]", tail):
            continue
        segments = data_segments(body)
        yield name, len(segments), text[: match.start()].count("\n") + 1, classify(name, segments)


def gather(root: pathlib.Path, max_params: int):
    violations = []
    for path in sorted(root.rglob("*")):
        if path.suffix not in {".h", ".hpp", ".cpp"}:
            continue
        for name, params, line, bucket in scan_file(path):
            if params > max_params:
                violations.append((path, line, name, params, bucket))
    return violations


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", default="Source/DemoProject/FrenchGulch")
    parser.add_argument("--max", type=int, default=2)
    parser.add_argument("--strict", action="store_true")
    args = parser.parse_args()

    violations = gather(pathlib.Path(args.root), args.max)
    actionable = [row for row in violations if row[4] == "actionable"]
    counts = Counter(row[4] for row in violations)

    print(f"Functions over {args.max} data params: {len(violations)}")
    print(f"  actionable:       {counts['actionable']}")
    print(f"  ue-callback:      {counts['ue-callback']}")
    print(f"  mut/engine sink:  {counts['mut-or-engine-sink']}")

    for path, line, name, params, bucket in actionable:
        print(f"{path}:{line}: {params} params: {name} [{bucket}]")

    return 1 if args.strict and actionable else 0


if __name__ == "__main__":
    sys.exit(main())
