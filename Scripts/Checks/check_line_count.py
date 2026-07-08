#!/usr/bin/env python3
"""Reject authored Source/Content files over the 300-line ceiling.

Analogous to the TS SDK 300-line rule (check-rtk-conformance) and the UE FP
CLI-handler rule (check-ue-fp-conformance Rule 4): a file over 300 lines is
doing too much. The remediation is the fp/rtk/ecs doctrine used everywhere else
in this suite -- split the file by subdomain into smaller role/domain files (or,
for authored data, smaller catalog/instance/subdomain JSON leaves) and recompose
them through an adapter/fold, so folder/namespace boundaries own the domain
words and each leaf stays small.

Do not split with ``.inl``/``.inc``/``.ipp`` fragments or other non-role
implementation leaves to bypass the RTK/ECS/View structure checks. Drill into
real descriptive subdomains instead, then keep code in checked ``.h``/``.hpp``/
``.cpp`` role/domain leaves.

Line counts are taken the same way as Scripts/Docs/count_project_lines.py (the
FILES_BY_LINE_NUMBER report), so the numbers agree. The guard always scans the
project Source and Content trees; path arguments are intentionally unsupported.
"""

from __future__ import annotations

import argparse
from pathlib import Path
import sys


PROJECT_ROOT = Path(__file__).resolve().parents[2]
SCAN_ROOTS = [PROJECT_ROOT / "Source", PROJECT_ROOT / "Content"]
TEXT_SUFFIXES = {
    ".h", ".hpp", ".cpp", ".c", ".cs", ".json", ".csv", ".ini", ".txt",
    ".inl", ".inc", ".ipp",
}
EXCLUDED_DIRS = {
    ".git", ".vs", ".vscode", "Binaries", "Build", "DerivedDataCache",
    "Intermediate", "Plugins", "Saved", "__pycache__",
}
MAX_LINES = 300

GUIDANCE = (
    "Split each file by subdomain and recompose through an adapter/fold. Source: "
    "smaller RTK/ECS role leaves or grouped declaration atoms fed to one reusable "
    "composer. Content: smaller catalog/instance/subdomain JSON leaves composed "
    "through the settings adapters. Folder/namespace boundaries own the domain "
    "words; each leaf stays small. Never use .inl/.inc/.ipp fragments or "
    "non-role implementation leaves as an escape hatch; drill into concrete "
    "subdomains and keep the result visible to the structural guards. A wide "
    "file is a bag hiding subdomains."
)


def has_excluded_part(path: Path) -> bool:
    return any(part in EXCLUDED_DIRS for part in path.parts)


def line_count(path: Path) -> int | None:
    try:
        with path.open("r", encoding="utf-8") as handle:
            return sum(1 for _ in handle)
    except (UnicodeDecodeError, OSError):
        return None


def iter_files() -> list[Path]:
    return sorted(
        path
        for root in SCAN_ROOTS
        if root.is_dir()
        for path in root.rglob("*")
        if path.is_file() and path.suffix in TEXT_SUFFIXES and not has_excluded_part(path)
    )


def find_over_limit() -> list[tuple[int, Path]]:
    over = [
        (count, path)
        for path in iter_files()
        for count in [line_count(path)]
        if count is not None and count > MAX_LINES
    ]
    return sorted(over, key=lambda item: item[0], reverse=True)


def display(path: Path) -> str:
    try:
        return path.relative_to(PROJECT_ROOT).as_posix()
    except ValueError:
        return path.as_posix()


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog="This guard always scans the project Source and Content trees; path arguments are intentionally unsupported.",
    )
    parser.parse_args()

    over = find_over_limit()
    if not over:
        print(f"Line-count guard passed. No Source/Content file exceeds {MAX_LINES} lines.")
        return 0

    print(f"Line-count guard failed: {len(over)} file(s) over {MAX_LINES} lines.")
    for count, path in over:
        print(f"{display(path)}: {count} lines, over the {MAX_LINES}-line limit; split into subdomains")
    print("")
    print(GUIDANCE)
    return 1


if __name__ == "__main__":
    sys.exit(main())
