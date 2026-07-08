#!/usr/bin/env python3
"""Reject hard-coded values in authored Source.

Numbers and strings that configure runtime behavior belong in JSON -- loaded
through the runtime settings adapters and read from typed settings/profile
structs -- not baked into C++ or headers. This guard scans all of Source and
flags every numeric literal and every string literal in authored code. The
project Source tree is always scanned, even if path arguments are supplied, so a
narrow path cannot make the guard appear green while authored Source still
contains hard-coded data.

The only exempt strings are action/slice/reducer metadata (action type strings,
slice names, thunk type prefixes, listener action types). That exempt set is
discovered DYNAMICALLY: the guard collects the string values actually passed to
the RTK creators in this codebase, rather than guessing an action-string format.
Comments and preprocessor lines (includes, pragmas, defines) are ignored.
Unreal reflection macro arguments are also ignored because metadata such as
`Category = "Level|Debug"` is consumed by UHT/editor tooling, not runtime
settings data.
"""

from __future__ import annotations

import argparse
from pathlib import Path
import re
import sys


SOURCE_SUFFIXES = {".h", ".hpp", ".cpp"}
PROJECT_ROOT = Path(__file__).resolve().parents[2]
PROJECT_SOURCE_ROOT = PROJECT_ROOT / "Source"

STRING_LITERAL = re.compile(r'"(?:[^"\\]|\\.)*"')
NUMERIC_LITERAL = re.compile(r"(?<![A-Za-z0-9_.])\d[0-9.]*")
UNREAL_REFLECTION_MACROS = {
    "UCLASS",
    "UDELEGATE",
    "UENUM",
    "UFUNCTION",
    "UINTERFACE",
    "UMETA",
    "UPARAM",
    "UPROPERTY",
    "USTRUCT",
}
UNREAL_REFLECTION_MACRO = re.compile(
    r"\b(" + "|".join(sorted(UNREAL_REFLECTION_MACROS)) + r")\s*\("
)
TEST_DECLARATION_MACRO = re.compile(
    r"\b(?:DEFINE_SPEC|IMPLEMENT_SIMPLE_AUTOMATION_TEST)\s*\("
)
TEST_METADATA_CALL = re.compile(r"\b(?:Describe|It)\s*\(")
JSON_SCHEMA_FIELD_CALL = re.compile(
    r"\b(?:[A-Za-z_][A-Za-z0-9_]*::)?(?:"
    r"Field|ReadStringField|ReadStringArrayField|ReadObjectField|"
    r"ReadObjectArrayField|ReadRequiredField|RequiredField|"
    r"RequiredFieldName|FindRequiredJsonField|ReadSettingsObjectArrayField|"
    r"SettingsSource|SettingsSourceKey|SettingsFieldName"
    r")\s*(?:<[^;{}()]*>)?\s*\("
)
RUNTIME_SETTINGS_SOURCE_GROUPS = re.compile(
    r"\bRuntimeSettingsSourceGroups\s*\("
)

# Where reducer/action metadata is defined. A string in the same statement as
# one of these (or in a `*TypePrefix` accessor) is action/slice/thunk metadata,
# not runtime configuration. The exempt VALUES are still collected dynamically
# from real usage below; this only identifies the definition sites.
METADATA_CONTEXT = re.compile(
    r"create(?:Action|Slice|Reducer|AsyncThunk)|addListener|TypePrefix"
)

GUIDANCE = (
    "Move the value into authored JSON under Content/Data, load it through "
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


def blank_unreal_reflection_metadata(text: str) -> str:
    """Blank Unreal reflection macro calls, preserving offsets and newlines."""
    result = list(text)
    cursor = 0
    while True:
        match = UNREAL_REFLECTION_MACRO.search(text, cursor)
        if not match:
            break

        index = match.end() - 1
        depth = 0
        state = "code"
        quote = ""
        while index < len(text):
            char = text[index]
            nxt = text[index + 1] if index + 1 < len(text) else ""
            if state == "string":
                escaped = char == "\\"
                state = "code" if (char == quote and not escaped) else state
                index += 2 if escaped else 1
                continue
            if state == "char":
                escaped = char == "\\"
                state = "code" if (char == "'" and not escaped) else state
                index += 2 if escaped else 1
                continue
            if char == '"':
                quote = char
                state = "string"
                index += 1
                continue
            if char == "'":
                state = "char"
                index += 1
                continue
            if char == "(":
                depth += 1
            elif char == ")":
                depth -= 1
                if depth == 0:
                    index += 1
                    break
            index += 1

        for offset in range(match.start(), index):
            result[offset] = "\n" if text[offset] == "\n" else " "
        cursor = index
    return "".join(result)


def call_end(text: str, open_paren: int) -> int:
    index = open_paren
    depth = 0
    state = "code"
    quote = ""
    while index < len(text):
        char = text[index]
        nxt = text[index + 1] if index + 1 < len(text) else ""
        if state == "string":
            escaped = char == "\\"
            state = "code" if (char == quote and not escaped) else state
            index += 2 if escaped else 1
            continue
        if state == "char":
            escaped = char == "\\"
            state = "code" if (char == "'" and not escaped) else state
            index += 2 if escaped else 1
            continue
        if char == '"':
            quote = char
            state = "string"
            index += 1
            continue
        if char == "'":
            state = "char"
            index += 1
            continue
        if char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
            if depth == 0:
                return index + 1
        index += 1
    return len(text)


def first_argument_end(text: str, open_paren: int, close_paren: int) -> int:
    index = open_paren + 1
    depth = 1
    state = "code"
    quote = ""
    while index < close_paren:
        char = text[index]
        nxt = text[index + 1] if index + 1 < len(text) else ""
        if state == "string":
            escaped = char == "\\"
            state = "code" if (char == quote and not escaped) else state
            index += 2 if escaped else 1
            continue
        if state == "char":
            escaped = char == "\\"
            state = "code" if (char == "'" and not escaped) else state
            index += 2 if escaped else 1
            continue
        if char == '"':
            quote = char
            state = "string"
            index += 1
            continue
        if char == "'":
            state = "char"
            index += 1
            continue
        if char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
        elif char == "," and depth == 1:
            return index
        index += 1
    return close_paren - 1


def blank_string_literals_in_range(result: list[str], text: str,
                                   start: int, end: int) -> None:
    for match in STRING_LITERAL.finditer(text, start, end):
        for offset in range(match.start(), match.end()):
            result[offset] = "\n" if text[offset] == "\n" else " "


def blank_test_metadata(text: str) -> str:
    """Blank test registration metadata while keeping assertion labels visible.

    Assertion labels are authored test data. They must remain scannable so
    strings like TestTrue(TEXT("Sorrel is projected into ECS"), ...) are caught
    by the hard-coded value gate.
    """
    result = list(text)

    for pattern in (TEST_DECLARATION_MACRO,):
        cursor = 0
        while True:
            match = pattern.search(text, cursor)
            if not match:
                break
            close = call_end(text, match.end() - 1)
            blank_string_literals_in_range(result, text, match.start(), close)
            cursor = close

    for pattern in (TEST_METADATA_CALL,):
        cursor = 0
        while True:
            match = pattern.search(text, cursor)
            if not match:
                break
            open_paren = match.end() - 1
            close = call_end(text, open_paren)
            first_end = first_argument_end(text, open_paren, close)
            blank_string_literals_in_range(result, text, open_paren, first_end)
            cursor = match.end()

    return "".join(result)


def body_end(text: str, open_brace: int) -> int:
    index = open_brace
    depth = 0
    state = "code"
    quote = ""
    while index < len(text):
        char = text[index]
        nxt = text[index + 1] if index + 1 < len(text) else ""
        if state == "string":
            escaped = char == "\\"
            state = "code" if (char == quote and not escaped) else state
            index += 2 if escaped else 1
            continue
        if state == "char":
            escaped = char == "\\"
            state = "code" if (char == "'" and not escaped) else state
            index += 2 if escaped else 1
            continue
        if char == '"':
            quote = char
            state = "string"
            index += 1
            continue
        if char == "'":
            state = "char"
            index += 1
            continue
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return index + 1
        index += 1
    return len(text)


def blank_settings_source_group_strings(result: list[str],
                                                text: str) -> None:
    cursor = 0
    while True:
        match = RUNTIME_SETTINGS_SOURCE_GROUPS.search(text, cursor)
        if not match:
            break
        close = call_end(text, match.end() - 1)
        open_brace = text.find("{", close)
        if open_brace == -1:
            cursor = close
            continue
        close_brace = body_end(text, open_brace)
        blank_string_literals_in_range(result, text, open_brace, close_brace)
        cursor = close_brace


def blank_json_schema_metadata(text: str) -> str:
    """Blank JSON field/schema identifiers while preserving JSON values."""
    result = list(text)

    cursor = 0
    while True:
        match = JSON_SCHEMA_FIELD_CALL.search(text, cursor)
        if not match:
            break
        close = call_end(text, match.end() - 1)
        blank_string_literals_in_range(result, text, match.start(), close)
        cursor = close

    blank_settings_source_group_strings(result, text)
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


def canonical_scan_paths(requested_paths: list[str]) -> list[Path]:
    paths = [PROJECT_SOURCE_ROOT.resolve()]
    paths.extend(Path(path).resolve() for path in requested_paths)

    deduped: list[Path] = []
    seen: set[Path] = set()
    for path in paths:
        if path in seen:
            continue
        seen.add(path)
        deduped.append(path)
    return deduped


def scan_paths(paths: list[Path]) -> list[tuple[Path, int, int, str, str]]:
    scannables = {}
    for path in iter_source_paths(paths):
        scannable = blank_unreal_reflection_metadata(
            blank_json_schema_metadata(
                blank_comments_and_chars(
                    blank_preprocessor(
                        path.read_text(encoding="utf-8", errors="replace")
                    )
                )
            )
        )
        scannables[path] = (
            blank_test_metadata(scannable)
            if "Tests" in path.parts
            else scannable
        )
    exempt = collect_metadata_strings(list(scannables.values()))
    return [
        violation
        for path, scannable in scannables.items()
        for violation in scan_file(path, scannable, exempt)
    ]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, add_help=False)
    # No --self-test flag, on purpose. A self-test only exercises the guard's own
    # fixture and prints "self-test passed" WITHOUT scanning your Source, so it
    # reads as green while real hard-coded values slip through untouched. The
    # guard must always scan a path (defaulting to Source). DO NOT add
    # --self-test back. Help is also not allowed to short-circuit the scan.
    parser.add_argument(
        "-h",
        "--help",
        action="store_true",
        help="Show this help message after the required Source scan passes.",
    )
    parser.add_argument(
        "paths",
        nargs="*",
        help=(
            "Extra C++ source file or directory to scan. Project Source is "
            "always scanned and cannot be replaced by this argument."
        ),
    )
    args = parser.parse_args()

    paths = canonical_scan_paths(args.paths)
    missing_paths = [path for path in paths if not path.exists()]
    if missing_paths:
        print("Hard-coded value guard failed:")
        for path in missing_paths:
            print(f"{path}: source path does not exist")
        if args.help:
            print("")
            parser.print_help()
        return 1

    violations = scan_paths(paths)
    if not violations:
        if args.help:
            parser.print_help()
            return 0
        print("Hard-coded value guard passed.")
        return 0

    print(f"Hard-coded value guard failed: {len(violations)} value(s).")
    for path, line, column, kind, token in violations:
        print(f"{path}:{line}:{column}: hard-coded {kind} `{token}`")
    print("")
    print(GUIDANCE)
    if args.help:
        print("")
        parser.print_help()
    return 1


if __name__ == "__main__":
    sys.exit(main())
