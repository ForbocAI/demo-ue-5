#!/usr/bin/env python3
"""Layer 1 dead-code / dead-data guard: orphan files.

Two orphan checks, both conservative (biased toward "alive" so a live file is
never falsely flagged -- deleting a wrongly-flagged file would break the build):

* DEAD-SRC-001  -- an authored header (.h/.hpp) that no OTHER file `#include`s.
  Headers are only ever used via `#include`, so an un-included header is dead.
  Exempted: UE reflection headers (UCLASS/UINTERFACE/GENERATED_BODY are
  engine/Blueprint/config-instantiated, not referenced by C++ text), the module
  header, and Tests (automation entry points).

* DEAD-DATA-001 -- an authored data file (Content/Data/**.json|.csv) whose
  Content-relative path ("Data/<rel>") and bare filename appear NOWHERE across
  Source, Content, and Scripts. Data is loaded through the settings/level
  manifest chain (path strings) and consumed by tool scripts, so all three trees
  form the reference universe.

The remediation for a real orphan is deletion (stale dead code/data). This guard
always scans the project trees; path arguments are intentionally unsupported.
"""

from __future__ import annotations

import argparse
from collections import defaultdict
from pathlib import Path
import re
import sys


PROJECT_ROOT = Path(__file__).resolve().parents[2]
SOURCE_ROOT = PROJECT_ROOT / "Source"
CONTENT_DATA_ROOT = PROJECT_ROOT / "Content" / "Data"
SCRIPTS_ROOT = PROJECT_ROOT / "Scripts"

SOURCE_SUFFIXES = {".h", ".hpp", ".cpp"}
HEADER_SUFFIXES = {".h", ".hpp"}
DATA_SUFFIXES = {".json", ".csv"}
SCRIPT_SUFFIXES = {".py", ".sh", ".ps1"}
EXCLUDED_DIRS = {
    ".git", ".vs", ".vscode", "Binaries", "Build", "DerivedDataCache",
    "Intermediate", "Plugins", "Saved", "__pycache__",
}

INCLUDE_RE = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.MULTILINE)
REFLECTION_RE = re.compile(r"\b(UCLASS|UINTERFACE|GENERATED_BODY)\b")
MODULE_ENTRY_STEMS = {"ForbocAIDemo"}


def has_excluded_part(path: Path) -> bool:
    return any(part in EXCLUDED_DIRS for part in path.parts)


def read(path: Path) -> str:
    try:
        return path.read_text(encoding="utf-8", errors="replace")
    except OSError:
        return ""


def iter_files(root: Path, suffixes: set[str]) -> list[Path]:
    if not root.is_dir():
        return []
    return sorted(
        path
        for path in root.rglob("*")
        if path.is_file() and path.suffix in suffixes and not has_excluded_part(path)
    )


def rel(path: Path) -> str:
    try:
        return path.relative_to(PROJECT_ROOT).as_posix()
    except ValueError:
        return path.as_posix()


# --- DEAD-SRC-001: orphan headers ------------------------------------------

def orphan_headers() -> list[Path]:
    source_files = iter_files(SOURCE_ROOT, SOURCE_SUFFIXES)
    texts = {path: read(path) for path in source_files}

    # basename/full-path -> set of files that #include it.
    includers: dict[str, set[Path]] = defaultdict(set)
    for path, text in texts.items():
        for target in INCLUDE_RE.findall(text):
            includers[target].add(path)
            includers[Path(target).name].add(path)

    orphans: list[Path] = []
    for path in source_files:
        if path.suffix not in HEADER_SUFFIXES:
            continue
        if "Tests" in path.parts or path.stem in MODULE_ENTRY_STEMS:
            continue
        if REFLECTION_RE.search(texts[path]):
            continue
        source_rel = path.relative_to(SOURCE_ROOT).as_posix()
        sibling_cpp = path.with_suffix(".cpp")
        refs = includers.get(source_rel, set()) | includers.get(path.name, set())
        if not (refs - {sibling_cpp}):
            orphans.append(path)
    return orphans


# --- DEAD-DATA-001: orphan data files --------------------------------------

def reference_blob() -> str:
    parts: list[str] = []
    for path in iter_files(SOURCE_ROOT, SOURCE_SUFFIXES):
        parts.append(read(path))
    for path in iter_files(SCRIPTS_ROOT, SCRIPT_SUFFIXES):
        parts.append(read(path))
    for path in iter_files(CONTENT_DATA_ROOT, DATA_SUFFIXES):
        parts.append(read(path))
    return "\n".join(parts)


def orphan_data() -> list[Path]:
    blob = reference_blob()
    orphans: list[Path] = []
    for path in iter_files(CONTENT_DATA_ROOT, DATA_SUFFIXES):
        content_rel = "Data/" + path.relative_to(CONTENT_DATA_ROOT).as_posix()
        # A data file never names itself; the blob-includes-self is harmless and
        # keeps the check conservative (alive if the name appears anywhere).
        if content_rel in blob or path.name in blob:
            continue
        orphans.append(path)
    return orphans


def main() -> int:
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog="This guard always scans the project trees; path arguments are intentionally unsupported.",
    )
    parser.parse_args()

    headers = orphan_headers()
    data = orphan_data()
    total = len(headers) + len(data)

    if total == 0:
        print("Dead-code guard passed. No orphan headers or data files.")
        return 0

    print(f"Dead-code guard failed: {total} orphan file(s) ({len(headers)} header(s), {len(data)} data file(s)).")
    for path in headers:
        print(f"{rel(path)}: DEAD-SRC-001 orphan header -- no other file #includes it; delete it or wire it in.")
    for path in data:
        print(f"{rel(path)}: DEAD-DATA-001 orphan data -- referenced by no Source/Content/Script; delete it or reference it.")
    print("")
    print(
        "An orphan file is stale dead code/data. Confirm it is truly unused (mind "
        "reflection, config, and dynamically built paths) and delete it, or wire "
        "it into an include / manifest / script that consumes it."
    )
    return 1


if __name__ == "__main__":
    sys.exit(main())
