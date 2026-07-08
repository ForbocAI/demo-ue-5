#!/usr/bin/env python3
"""Reject non-FP control-flow statements in authored Source files."""

from __future__ import annotations

import argparse
from pathlib import Path
import re
import sys


SOURCE_SUFFIXES = {".h", ".hpp", ".cpp", ".cs"}
CONTROL_PATTERN = re.compile(r"\b(if|else|for|while|switch|case)\b")

TOKEN_GUIDANCE = {
    "if": (
        "Refactor conditionals into func::match for Maybe/Either/result "
        "values, func::multi_match/Dispatcher tables for routed values, or "
        "data-authored predicates interpreted by reusable functions."
    ),
    "else": (
        "Refactor paired branches into func::match, ternary expressions for "
        "small value selection, or func::multi_match cases with explicit "
        "required matches checked at the boundary."
    ),
    "switch": (
        "Refactor switch routing into func::multi_match or Dispatcher case "
        "tables. Keep enum/action identifiers as code metadata; put runtime "
        "display strings and values in JSON."
    ),
    "case": (
        "Refactor case arms into func::when handlers inside func::multi_match "
        "or a data table interpreted by composable helpers."
    ),
    "for": (
        "Refactor loops into SDK functional-core fold/filter/find/traverse "
        "helpers such as fold_indexed, for_each_indexed, find_indexed, "
        "any_indexed, all_indexed, or map_grid."
    ),
    "while": (
        "Refactor open-ended loops into SDK functional-core recursion, folds, "
        "AsyncChain composition, or explicit state-machine data interpreted by "
        "one reusable step function."
    ),
}

BOUNDARY_GUIDANCE = (
    "Functional-core cookbook boundary: use SDK func::* helpers for non-ECS "
    "collection and decision work. Use ECS helpers only for ECS-owned "
    "entity/component/system world transitions. At UE/effect boundaries, "
    "prefer required-data checks with check(...)."
)


def is_source_file(path: Path) -> bool:
    return path.suffix in SOURCE_SUFFIXES


def strip_comments_and_strings(text: str) -> str:
    result: list[str] = []
    index = 0
    state = "code"
    quote = ""

    while index < len(text):
        char = text[index]
        next_char = text[index + 1] if index + 1 < len(text) else ""

        if state == "line_comment":
            result.append("\n" if char == "\n" else " ")
            state = "code" if char == "\n" else state
            index += 1
            continue

        if state == "block_comment":
            result.append("\n" if char == "\n" else " ")
            state = "code" if char == "*" and next_char == "/" else state
            index += 2 if char == "*" and next_char == "/" else 1
            continue

        if state == "string":
            result.append("\n" if char == "\n" else " ")
            escaped = char == "\\" and next_char
            closed = char == quote and not escaped
            state = "code" if closed else state
            index += 2 if escaped else 1
            continue

        if char == "/" and next_char == "/":
            result.extend("  ")
            state = "line_comment"
            index += 2
            continue

        if char == "/" and next_char == "*":
            result.extend("  ")
            state = "block_comment"
            index += 2
            continue

        if char in {"'", '"'}:
            result.append(" ")
            quote = char
            state = "string"
            index += 1
            continue

        result.append(char)
        index += 1

    return "".join(result)


def line_for_offset(text: str, offset: int) -> int:
    return text.count("\n", 0, offset) + 1


def scan_file(path: Path, root: Path) -> list[tuple[str, int, str]]:
    text = path.read_text(encoding="utf-8", errors="ignore")
    searchable = strip_comments_and_strings(text)
    relative = path.relative_to(root).as_posix()
    return [
        (relative, line_for_offset(searchable, match.start()), match.group(1))
        for match in CONTROL_PATTERN.finditer(searchable)
    ]


def scan_source(root: Path) -> list[tuple[str, int, str]]:
    return [
        violation
        for path in sorted(root.rglob("*"))
        if path.is_file() and is_source_file(path)
        for violation in scan_file(path, root)
    ]


def guidance_for(token: str) -> str:
    return TOKEN_GUIDANCE[token]


def run_self_test() -> bool:
    samples = {
        "if": "void x(){ if (Ready) { Run(); } }",
        "else": "void x(){ Ready ? Run() : Stop(); else; }",
        "switch": "void x(){ switch(Value) {} }",
        "case": "void x(){ switch(Value) { case 1: break; } }",
        "for": "void x(){ for (int i = 0; i < 1; ++i) { Run(); } }",
        "while": "void x(){ while (Ready) { Run(); } }",
    }
    violations = [
        (f"{token}.cpp", line_for_offset(strip_comments_and_strings(source), match.start()), match.group(1))
        for token, source in samples.items()
        for match in CONTROL_PATTERN.finditer(strip_comments_and_strings(source))
    ]
    found_tokens = {token for _, _, token in violations}
    guidance_ok = all(guidance_for(token) for token in samples)
    return found_tokens == set(samples) and guidance_ok


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--self-test",
        action="store_true",
        help="Run guard fixture checks for each rejected control-flow token.",
    )
    parser.add_argument(
        "source_root",
        nargs="?",
        default="Source",
        help="Source directory to scan.",
    )
    args = parser.parse_args()

    if args.self_test:
        if run_self_test():
            print("Branchless Source guard self-test passed.")
            return 0
        print("Branchless Source guard self-test failed.")
        return 1

    source_root = Path(args.source_root).resolve()
    violations = scan_source(source_root)

    if not violations:
        print("Branchless Source guard passed.")
        return 0

    print("Branchless Source guard failed:")
    for path, line, token in violations:
        print(f"{path}:{line}: non-FP `{token}` statement")
        print(f"  {guidance_for(token)}")
    print("")
    print(BOUNDARY_GUIDANCE)
    return 1


if __name__ == "__main__":
    sys.exit(main())
