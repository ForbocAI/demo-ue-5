#!/usr/bin/env python3
"""Reject hard-coded runtime rendering tuning values in authored C++ helpers."""

from __future__ import annotations

import argparse
from pathlib import Path
import re
import sys


SOURCE_SUFFIXES = {".h", ".hpp", ".cpp"}
NUMERIC_LITERAL_PATTERN = re.compile(
    r"(?<![A-Za-z0-9_])(?:\d+\.\d+f?|\d+f?)(?![A-Za-z0-9_])"
)
TUNING_FUNCTIONS = {
    "RuntimeMoonPixelGridSize",
    "RuntimeMoonPixelIndices",
    "RuntimeMoonPixelWorldDiameter",
    "RuntimeMoonPixelCellWorldSize",
    "RuntimeMoonPixelQuad",
    "RuntimePointStarCount",
    "RuntimePointStarYawDegrees",
    "RuntimePointStarPitchDegrees",
    "RuntimePointStarDistance",
    "RuntimePointStarWorldSize",
    "RuntimePointStarQuad",
    "RuntimePointStarMeshBuffers",
}
GUIDANCE = (
    "Move runtime rendering tuning values to "
    "Content/Data/runtime_settings_rendering_profile.json and expose them "
    "through FLevelRetroRenderProfile."
)


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


def matching_delimiter(text: str, open_index: int, open_char: str, close_char: str) -> int | None:
    depth = 1
    index = open_index + 1
    while index < len(text) and depth > 0:
        char = text[index]
        depth += 1 if char == open_char else 0
        depth -= 1 if char == close_char else 0
        index += 1
    return index - 1 if depth == 0 else None


def line_column(text: str, offset: int) -> tuple[int, int]:
    line_start = text.rfind("\n", 0, offset) + 1
    return text.count("\n", 0, offset) + 1, offset - line_start + 1


def function_ranges(text: str, function_name: str) -> list[tuple[int, int]]:
    ranges: list[tuple[int, int]] = []
    pattern = re.compile(rf"\b{re.escape(function_name)}\s*\(")
    for match in pattern.finditer(text):
        open_paren = text.find("(", match.start())
        close_paren = matching_delimiter(text, open_paren, "(", ")")
        if close_paren is None:
            continue
        body_start = close_paren + 1
        while body_start < len(text) and text[body_start].isspace():
            body_start += 1
        if body_start >= len(text) or text[body_start] != "{":
            continue
        body_end = matching_delimiter(text, body_start, "{", "}")
        if body_end is not None:
            ranges.append((body_start, body_end + 1))
    return ranges


def scan_text(path: Path, text: str) -> list[tuple[Path, int, int, str, str]]:
    searchable = strip_comments_and_strings(text)
    return [
        (path, *line_column(searchable, match.start()), match.group(0), function)
        for function in sorted(TUNING_FUNCTIONS)
        for start, end in function_ranges(searchable, function)
        for match in NUMERIC_LITERAL_PATTERN.finditer(searchable, start, end)
    ]


def iter_source_paths(paths: list[Path]) -> list[Path]:
    return [
        source
        for path in paths
        for source in (
            sorted(path.rglob("*")) if path.is_dir() else [path]
        )
        if source.is_file() and source.suffix in SOURCE_SUFFIXES
    ]


def scan_paths(paths: list[Path]) -> list[tuple[Path, int, int, str, str]]:
    return [
        violation
        for path in iter_source_paths(paths)
        for violation in scan_text(
            path, path.read_text(encoding="utf-8", errors="replace")
        )
    ]


def run_self_test() -> bool:
    fixture = """
int32 RuntimePointStarCount() { return 260; }
float RuntimePointStarDistance(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonDiscDistance * Profile.PointStarDistanceMultiplier;
}
const TCHAR *IgnoredString = TEXT("512");
// RuntimeMoonPixelGridSize returns 32 in old code.
"""
    violations = scan_text(Path("fixture.cpp"), fixture)
    return len(violations) == 1 and violations[0][3] == "260"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--self-test",
        action="store_true",
        help="Run guard fixture checks for hard-coded rendering tuning values.",
    )
    parser.add_argument(
        "paths",
        nargs="*",
        default=["Source"],
        help="C++ source file or directory to scan.",
    )
    args = parser.parse_args()

    if args.self_test:
        if run_self_test():
            print("Runtime rendering JSON tuning guard self-test passed.")
            return 0
        print("Runtime rendering JSON tuning guard self-test failed.")
        return 1

    paths = [Path(path).resolve() for path in args.paths]
    missing_paths = [path for path in paths if not path.exists()]
    if missing_paths:
        print("Runtime rendering JSON tuning guard failed:")
        for path in missing_paths:
            print(f"{path}: source path does not exist")
        return 1

    violations = scan_paths(paths)
    if not violations:
        print("Runtime rendering JSON tuning guard passed.")
        return 0

    print("Runtime rendering JSON tuning guard failed:")
    for path, line, column, token, function in violations:
        print(
            f"{path}:{line}:{column}: hard-coded numeric tuning literal "
            f"`{token}` in {function}"
        )
        print(f"  {GUIDANCE}")
    return 1


if __name__ == "__main__":
    sys.exit(main())
