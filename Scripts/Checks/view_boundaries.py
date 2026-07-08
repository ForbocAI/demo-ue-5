#!/usr/bin/env python3
"""Reject Source/Views files that bypass feature action/selector boundaries.

Views are the presentation boundary, not an RTK feature role. They may render,
bind UE lifecycle callbacks, and dispatch user/engine events through feature
Actions. They may read already-derived data through feature Selectors. They may
not import Types, Slice, Thunks, Listeners, or Adapters directly; those roles
belong behind feature facades so domains can move without view rewrites.
"""

from __future__ import annotations

import argparse
from pathlib import Path
import re
import sys

from boundary_common import SOURCE_EXTENSIONS, line_number


FEATURE_INCLUDE = re.compile(r'^\s*#\s*include\s+"(Features/[^"]+)"')
STORE_INCLUDE = re.compile(r'^\s*#\s*include\s+"Store\.h"')
STORE_ACCESS = re.compile(r"\bStore::GetStore\s*\(|\.dispatch\s*\(|\.getState\s*\(")
RTK_QUERY_CACHE_PATCH = re.compile(r"\bupdateQueryData\s*\(|\bselectFromResult\s*\(")
DIRECT_ASYNC_WORKFLOW = re.compile(
    r"\brtk::createAsyncThunk\s*<|\bcreateListenerMiddleware\s*<|\bStartListening\b"
)
WHOLE_STATE_SELECTOR = re.compile(r"\bSelect[A-Za-z0-9_]*State\s*\(")
BOUNDARY_IO = re.compile(
    r"\bIFileManager::|\bFPaths::|\bFParse::|\bFCommandLine::|\bFHttpModule\b|\bIHttpRequest\b"
)
GUIDANCE = (
    "Views may include only feature Actions or Selectors facades. Move data "
    "derivation into Selectors, event creation/dispatch into Actions, async "
    "workflows into Thunks, reactive workflows into Listeners, and JSON/UE/ECS "
    "translation into Adapters before the View consumes the result. The view "
    "subject belongs in folders; the file leaf must be exactly View."
)


def finding(path: Path, views_root: Path, line: int, message: str) -> tuple[Path, int, str]:
    return path.relative_to(views_root), line, message


def iter_source_paths(root: Path) -> list[Path]:
    return [
        path
        for path in sorted(root.rglob("*"))
        if path.is_file() and path.suffix in SOURCE_EXTENSIONS
    ]


def check_view_name(path: Path, views_root: Path) -> list[tuple[Path, int, str]]:
    return (
        []
        if path.stem == "View"
        else [
            finding(
                path,
                views_root,
                1,
                "view source file leaf must be exactly `View`; move subject words into folders",
            )
        ]
    )


def is_allowed_feature_include(include: str) -> bool:
    leaf = Path(include).name
    return leaf in {"Actions.h", "Selectors.h"}


def check_feature_includes(path: Path, views_root: Path) -> list[tuple[Path, int, str]]:
    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    feature_violations = [
        finding(
            path,
            views_root,
            line,
            f"disallowed feature include `{include}`",
        )
        for line, value in enumerate(lines, 1)
        for match in [FEATURE_INCLUDE.match(value)]
        if match
        for include in [match.group(1)]
        if not is_allowed_feature_include(include)
    ]
    store_violations = [
        finding(
            path,
            views_root,
            line,
            "views must not import Store.h; use feature Actions and Selectors",
        )
        for line, value in enumerate(lines, 1)
        if STORE_INCLUDE.match(value)
    ]
    return feature_violations + store_violations


def check_view_source(path: Path, views_root: Path) -> list[tuple[Path, int, str]]:
    text = path.read_text(encoding="utf-8", errors="replace")
    findings: list[tuple[Path, int, str]] = []

    for match in STORE_ACCESS.finditer(text):
        findings.append(
            finding(
                path,
                views_root,
                line_number(text, match.start()),
                "views must not access the store directly; dispatch through Actions and read through Selectors",
            )
        )

    for match in DIRECT_ASYNC_WORKFLOW.finditer(text):
        findings.append(
            finding(
                path,
                views_root,
                line_number(text, match.start()),
                "views must not create RTK async/listener workflows; move imperative work to Thunks and reactive work to Listeners",
            )
        )

    for pattern, message in (
        (
            RTK_QUERY_CACHE_PATCH,
            "View-boundary violation, forward target: RTK Query cache patches live in endpoint lifecycles, not presentation code",
        ),
        (
            WHOLE_STATE_SELECTOR,
            "View-boundary violation, forward target: views subscribe to narrow selector results, not whole slice/state objects",
        ),
        (
            BOUNDARY_IO,
            "View-boundary violation, forward target: file, command-line, and network IO lives behind Adapters, Thunks, or Listeners before a View consumes the result",
        ),
    ):
        match = pattern.search(text)
        if match:
            findings.append(
                finding(path, views_root, line_number(text, match.start()), message)
            )

    return findings


def check_views(views_root: Path) -> list[tuple[Path, int, str]]:
    return [
        violation
        for path in iter_source_paths(views_root)
        for violation in (
            check_view_name(path, views_root)
            + check_feature_includes(path, views_root)
            + check_view_source(path, views_root)
        )
    ]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "views_root",
        nargs="?",
        default="Source/Views",
        help="View source directory to scan.",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    views_root = Path(args.views_root).resolve()
    if not views_root.exists():
        print(f"View boundary guard failed: {views_root} does not exist")
        return 1

    findings = check_views(views_root)
    if not findings:
        print("View boundary guard passed.")
        return 0

    print(f"View boundary guard failed: {len(findings)} issue(s).")
    for relative, line, message in findings:
        print(f"{relative.as_posix()}:{line}: {message}")
    print("")
    print(GUIDANCE)
    return 1


if __name__ == "__main__":
    sys.exit(main())
