#!/usr/bin/env python3
"""Reject hard-coded values in authored Source.

Numbers and strings that configure runtime behavior belong in JSON -- loaded
through the runtime settings adapters and read from typed settings/profile
structs -- not baked into C++ or headers. This guard scans all of Source and
flags every numeric literal and every string literal in authored code.

The only exempt strings are action/slice/reducer metadata (action type strings,
slice names, thunk type prefixes, listener action types). That exempt set is
discovered DYNAMICALLY: the guard collects the string values actually passed to
the RTK creators in this codebase, rather than guessing an action-string format.
Comments and preprocessor lines (includes, pragmas, defines) are ignored.
"""

from __future__ import annotations

import argparse
from pathlib import Path
import re
import sys


SOURCE_SUFFIXES = {".h", ".hpp", ".cpp"}

STRING_LITERAL = re.compile(r'"(?:[^"\\]|\\.)*"')
NUMERIC_LITERAL = re.compile(r"(?<![A-Za-z0-9_.])\d[0-9.]*")

# Where reducer/action metadata is defined. A string in the same statement as
# one of these (or in a `*TypePrefix` accessor) is action/slice/thunk metadata,
# not runtime configuration. The exempt VALUES are still collected dynamically
# from real usage below; this only identifies the definition sites.
METADATA_CONTEXT = re.compile(
    r"create(?:Action|Slice|Reducer|AsyncThunk)|addListener|TypePrefix"
)

GUIDANCE = (
    "Move the value into Content/Data/runtime_settings_*.json, load it through "
    "the runtime settings adapters, and read it from a typed settings/profile "
    "struct. Only action type strings, slice names, and thunk type prefixes may "
    "remain in code as reducer/action metadata (auto-detected from RTK usage)."
)


def blank_preprocessor(text: str) -> str:
    """Replace preprocessor directive lines with spaces, preserving offsets."""
    return "\n".join(
        (" " * len(line)) if line.lstrip().startswith("#") else line
        for line in text.split("\n")
    )


def blank_comments_and_chars(text: str) -> str:
    """Blank comments and char literals, keeping string literals and code.

    Every replacement preserves length and newlines so offsets still map to the
    original line/column.
    """
    result: list[str] = []
    index = 0
    state = "code"
    quote = ""
    while index < len(text):
        char = text[index]
        nxt = text[index + 1] if index + 1 < len(text) else ""
        if state == "line_comment":
            result.append("\n" if char == "\n" else " ")
            state = "code" if char == "\n" else state
            index += 1
            continue
        if state == "block_comment":
            end = char == "*" and nxt == "/"
            result.append("  " if end else ("\n" if char == "\n" else " "))
            index += 2 if end else 1
            state = "code" if end else state
            continue
        if state == "string":
            escaped = char == "\\"
            result.append(char)
            result.append(nxt) if escaped else None
            state = "code" if (char == quote and not escaped) else state
            index += 2 if escaped else 1
            continue
        if state == "char":
            escaped = char == "\\"
            result.append("  " if escaped else ("'" if char == "'" else " "))
            state = "code" if (char == "'" and not escaped) else state
            index += 2 if escaped else 1
            continue
        if char == "/" and nxt == "/":
            result.extend("  ")
            state = "line_comment"
            index += 2
            continue
        if char == "/" and nxt == "*":
            result.extend("  ")
            state = "block_comment"
            index += 2
            continue
        if char == '"':
            quote = char
            state = "string"
            result.append(char)
            index += 1
            continue
        if char == "'":
            state = "char"
            result.append("'")
            index += 1
            continue
        result.append(char)
        index += 1
    return "".join(result)


def line_column(text: str, offset: int) -> tuple[int, int]:
    line_start = text.rfind("\n", 0, offset) + 1
    return text.count("\n", 0, offset) + 1, offset - line_start + 1


def collect_metadata_strings(scannables: list[str]) -> set[str]:
    """Discover action/slice/thunk metadata string values from real RTK usage."""
    exempt: set[str] = set()
    for scannable in scannables:
        for match in STRING_LITERAL.finditer(scannable):
            preceding = scannable[max(0, match.start() - 200): match.start()]
            if METADATA_CONTEXT.search(preceding):
                exempt.add(match.group(0)[1:-1])
    return exempt


def scan_file(
    path: Path, scannable: str, exempt: set[str]
) -> list[tuple[Path, int, int, str, str]]:
    violations: list[tuple[Path, int, int, str, str]] = []

    code_without_strings = list(scannable)
    for match in STRING_LITERAL.finditer(scannable):
        value = match.group(0)[1:-1]
        for index in range(match.start(), match.end()):
            code_without_strings[index] = " "
        if value == "" or value in exempt:
            continue
        line, column = line_column(scannable, match.start())
        violations.append((path, line, column, "string", match.group(0)))

    code_only = "".join(code_without_strings)
    for match in NUMERIC_LITERAL.finditer(code_only):
        line, column = line_column(code_only, match.start())
        violations.append((path, line, column, "number", match.group(0)))

    return violations


def iter_source_paths(paths: list[Path]) -> list[Path]:
    return [
        source
        for path in paths
        for source in (sorted(path.rglob("*")) if path.is_dir() else [path])
        if source.is_file() and source.suffix in SOURCE_SUFFIXES
    ]


def scan_paths(paths: list[Path]) -> list[tuple[Path, int, int, str, str]]:
    scannables = {
        path: blank_comments_and_chars(
            blank_preprocessor(path.read_text(encoding="utf-8", errors="replace"))
        )
        for path in iter_source_paths(paths)
    }
    exempt = collect_metadata_strings(list(scannables.values()))
    return [
        violation
        for path, scannable in scannables.items()
        for violation in scan_file(path, scannable, exempt)
    ]


def run_self_test() -> bool:
    fixture = """
#include "Ignored/Path/5.8.h"
int32 RuntimePointStarCount() { return 260; }
const TCHAR *Name = TEXT("BaseColor");
// RuntimeMoonPixelGridSize returns 32 in old code.
auto Slice = rtk::createSlice<FRuntimeState>(TEXT("systems/rendering"), S);
auto Type = createAction<FPayload>(TEXT("botAI/seeded"));
FString RequestPlayerSpawnTypePrefix() { return TEXT("runtime/spawn"); }
FRotator TopDown = FRotator(-90.0f, 0.0f, 0.0f);
"""
    scannable = blank_comments_and_chars(blank_preprocessor(fixture))
    exempt = collect_metadata_strings([scannable])
    found = scan_file(Path("fixture.cpp"), scannable, exempt)
    numbers = {token for _p, _l, _c, kind, token in found if kind == "number"}
    strings = {token for _p, _l, _c, kind, token in found if kind == "string"}
    return (
        "260" in numbers
        and "90.0" in numbers            # scale-audit tuning literal
        and "32" not in numbers          # inside a comment
        and "5.8" not in numbers         # preprocessor include line
        and '"BaseColor"' in strings
        # action/slice/thunk metadata dynamically collected -> exempt:
        and "systems/rendering" in exempt
        and "botAI/seeded" in exempt
        and "runtime/spawn" in exempt
        and not any("systems/rendering" in value for value in strings)
        and not any("botAI/seeded" in value for value in strings)
    )


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--self-test",
        action="store_true",
        help="Run guard fixture checks for hard-coded values.",
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
            print("Hard-coded value guard self-test passed.")
            return 0
        print("Hard-coded value guard self-test failed.")
        return 1

    paths = [Path(path).resolve() for path in args.paths]
    missing_paths = [path for path in paths if not path.exists()]
    if missing_paths:
        print("Hard-coded value guard failed:")
        for path in missing_paths:
            print(f"{path}: source path does not exist")
        return 1

    violations = scan_paths(paths)
    if not violations:
        print("Hard-coded value guard passed.")
        return 0

    print(f"Hard-coded value guard failed: {len(violations)} value(s).")
    for path, line, column, kind, token in violations:
        print(f"{path}:{line}:{column}: hard-coded {kind} `{token}`")
    print("")
    print(GUIDANCE)
    return 1


if __name__ == "__main__":
    sys.exit(main())
