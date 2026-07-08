#!/usr/bin/env python3
"""Shared primitives for RTK/ECS boundary guards."""

from __future__ import annotations

import re
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


SOURCE_EXTENSIONS = {".h", ".hpp", ".cpp"}
IMPORT_RE = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.MULTILINE)

ROLE_BY_STEM: dict[str, str] = {
    "Actions": "actions",
    "Adapters": "adapters",
    "Listeners": "listeners",
    "Selectors": "selectors",
    "Slice": "slice",
    "Thunks": "thunks",
    "Types": "types",
}

ROLE_GUIDANCE: dict[str, str] = {
    "actions": "Actions describe events or expose narrow UE dispatch facades; reducers own the state transition.",
    "adapters": "Adapters translate authored JSON, UE objects, or ECS data at boundaries without dispatching.",
    "listeners": "Listeners model reactive workflows that respond to future actions or state changes.",
    "selectors": "Selectors derive state and keep views from reading store internals directly.",
    "slice": "Slice owns createSlice assembly, initial state, and pure reducer transition functions.",
    "thunks": "Thunks model one imperative async workflow with dispatch/getState.",
    "types": "Types stay inert: state, payload, entity, and dispatch declarations only.",
}


@dataclass(frozen=True)
class Issue:
    path: Path
    line: int
    message: str


def role_for_stem(stem: str) -> str | None:
    return ROLE_BY_STEM.get(stem)


def role_for_include(include: str) -> str | None:
    if not include.startswith("Features/"):
        return None
    return role_for_stem(Path(include).stem)


def feature_relative_include(include: str) -> Path | None:
    if not include.startswith("Features/"):
        return None
    return Path(include).relative_to("Features")


def iter_source_files(roots: Iterable[Path]) -> Iterable[Path]:
    for root in roots:
        path = root.resolve()
        if path.is_file() and path.suffix in SOURCE_EXTENSIONS:
            yield path
        elif path.is_dir():
            yield from sorted(
                child
                for child in path.rglob("*")
                if child.is_file() and child.suffix in SOURCE_EXTENSIONS
            )


def line_number(text: str, index: int) -> int:
    return text.count("\n", 0, index) + 1


def first_line_for_pattern(text: str, pattern: re.Pattern[str]) -> int:
    match = pattern.search(text)
    return line_number(text, match.start()) if match else 1


def sibling_text(path: Path) -> str:
    if path.suffix not in {".h", ".hpp"}:
        return ""
    sibling = path.with_suffix(".cpp")
    if not sibling.exists():
        return ""
    return sibling.read_text(encoding="utf-8", errors="replace")


def combined_role_text(path: Path, text: str) -> str:
    return text + "\n" + sibling_text(path)


def pattern_issue(
    path: Path,
    text: str,
    pattern: re.Pattern[str],
    message: str,
) -> Issue | None:
    match = pattern.search(text)
    if not match:
        return None
    return Issue(path, line_number(text, match.start()), message)
