#!/usr/bin/env python3
"""Aggregate RTK/ECS feature boundary guard for Source/Features.

This file owns cross-role structure: valid feature leaves, import direction,
the single store boundary, dispatch/getState placement, and shared side-effect
rules. Role-specific RTK guidance lives in:

* action_boundaries.py
* adapter_boundaries.py
* listener_boundaries.py
* selector_boundaries.py
* slice_boundaries.py
* thunk_boundaries.py
* types_boundaries.py

The split keeps domains fluid. Domain words stay in folders; role files stay
small and semantic.
"""

from __future__ import annotations

import argparse
import os
from pathlib import Path
import re
import sys

import action_boundaries
import adapter_boundaries
from boundary_common import (
    IMPORT_RE,
    ROLE_GUIDANCE,
    WARNING_TRUE_VALUES,
    Issue,
    first_line_for_pattern,
    iter_source_files,
    line_number,
    role_for_include,
    role_for_stem,
)
import listener_boundaries
import selector_boundaries
import slice_boundaries
import thunk_boundaries
import types_boundaries


FORBIDDEN_STEM_SUFFIX_GUIDANCE: tuple[tuple[str, str], ...] = (
    (
        "Reducers",
        "Move pure reducer functions into the sibling Slice file. RTK reducers are slice-owned transitions; keep them pure and keep side effects in Thunks or Listeners.",
    ),
    (
        "Factories",
        "Replace Factories with the owning role: Slice for initial state, Adapters for JSON/UE/ECS translation, or Thunks/Listeners for effectful workflows.",
    ),
    (
        "Dispatch",
        "Replace Dispatch files with Actions, Thunks, or Listeners. Keep one store boundary and dispatch event-style actions instead of adding dispatch modules.",
    ),
    (
        "StateTypes",
        "Move the domain words into folders and keep the leaf as Types.h, for example State/Types.h.",
    ),
    (
        "PayloadTypes",
        "Move the domain words into folders and keep the leaf as Types.h, for example Payload/Types.h.",
    ),
    (
        "DataTypes",
        "Move the domain words into folders and keep the leaf as Types.h, for example Data/Types.h.",
    ),
    (
        "ColorTypes",
        "Move the domain words into folders and keep the leaf as Types.h, for example Color/Types.h.",
    ),
    (
        "AdaptersTypes",
        "Move the domain words into folders and keep the leaf as Types.h, for example Adapters/Types.h.",
    ),
    (
        "AdaptersSelectors",
        "Move the domain words into folders and keep the leaf as Selectors.h, for example Adapters/Selectors.h.",
    ),
)

# Import rules keep roles pointed in RTK dataflow direction:
# action -> slice/reducer -> selector -> view, with thunks/listeners owning effects.
FORBIDDEN_TARGET_ROLES: dict[str, set[str]] = {
    "types": {"actions", "selectors", "thunks", "listeners", "adapters", "slice"},
    "actions": {"selectors", "listeners"},
    "adapters": {"actions", "listeners"},
    "selectors": {"actions", "thunks", "listeners"},
    "slice": {"listeners"},
}

STORE_ACCESS_ALLOWLIST = {
    Path("Systems/Actions.cpp"),
    Path("Systems/Listeners.cpp"),
    Path("Systems/Selectors.cpp"),
    Path("Systems/Thunks.cpp"),
}

RAW_DISPATCH_ALLOW_ROLES = {"actions", "thunks", "listeners"}
RAW_GET_STATE_ALLOW_ROLES = {"thunks", "listeners"}

ROLE_MODULES = {
    "actions": action_boundaries,
    "adapters": adapter_boundaries,
    "listeners": listener_boundaries,
    "selectors": selector_boundaries,
    "slice": slice_boundaries,
    "thunks": thunk_boundaries,
    "types": types_boundaries,
}

RTK_FACTORY_RULES = tuple(
    rule
    for module in ROLE_MODULES.values()
    for rule in getattr(module, "OWNED_FACTORY_RULES", ())
)

REDUCER_SIDE_EFFECT_PATTERNS: tuple[tuple[re.Pattern[str], str], ...] = (
    (re.compile(r"\bLoadObject\s*<"), "Slice reducer logic, Selectors, and Types should not load Unreal assets"),
    (re.compile(r"\bSpawnActor\s*<"), "Slice reducer logic, Selectors, and Types should not spawn actors"),
    (re.compile(r"\bGetTimerManager\s*\("), "Slice reducer logic, Selectors, and Types should not schedule timers"),
    (re.compile(r"\bFScreenshotRequest::"), "Slice reducer logic, Selectors, and Types should not request screenshots"),
    (re.compile(r"\bFParse::|\bFCommandLine::"), "Slice reducer logic, Selectors, and Types should not parse command lines"),
    (re.compile(r"\bIFileManager::|\bFPaths::"), "Slice reducer logic, Selectors, and Types should not perform filesystem IO"),
    (re.compile(r"\bConsoleCommand\s*\("), "Slice reducer logic, Selectors, and Types should not issue console commands"),
)

RTK_QUERY_PATTERN = re.compile(r"\bcreateApi\s*<|\binjectEndpoints\s*\(")
RTK_QUERY_LIFECYCLE_PATTERN = re.compile(
    r"\bprovidesTags\b|\binvalidatesTags\b|\bonQueryStarted\b|\bonCacheEntryAdded\b"
)

STORE_FORBIDDEN_PATTERN_RULES: tuple[tuple[re.Pattern[str], str], ...] = (
    (
        re.compile(r"\bcreateStore\s*\(|\bapplyMiddleware\s*\("),
        "Use the modern configureStore/enhanced store boundary instead of createStore/applyMiddleware boilerplate.",
    ),
)

STORE_CONFIGURE_PATTERN = re.compile(r"\brtk::configureStore\s*<|(?<!create)configureStore\s*\(")
WARNING_AS_ERROR_ENV = "FORBOC_RTK_WARNINGS_AS_ERRORS"


def role_name_guidance(stem: str) -> str:
    for suffix, guidance in FORBIDDEN_STEM_SUFFIX_GUIDANCE:
        if stem.endswith(suffix):
            return guidance
    return (
        "Feature file leaves must be exact RTK/ECS role names: Actions, "
        "Adapters, Listeners, Selectors, Slice, Thunks, or Types. Move domain "
        "and subdomain words into folders so boundaries can shift without "
        "renaming role files."
    )


def contains_forbidden_include(include: str, source_role: str) -> str | None:
    if include.startswith("Views/") or "/Views/" in include:
        return "features must not import Views"
    target_role = role_for_include(include)
    if target_role and target_role in FORBIDDEN_TARGET_ROLES.get(source_role, set()):
        return (
            f"{source_role} files must not import {target_role} files. "
            f"{ROLE_GUIDANCE[source_role]} {ROLE_GUIDANCE[target_role]}"
        )
    return None


def check_imports(path: Path, text: str, role: str, rel: Path) -> list[Issue]:
    issues: list[Issue] = []
    for match in IMPORT_RE.finditer(text):
        include = match.group(1)
        reason = contains_forbidden_include(include, role)
        if reason:
            issues.append(Issue(path, line_number(text, match.start()), f"{reason}: `{include}`"))
        if include == "Store.h" and rel not in STORE_ACCESS_ALLOWLIST:
            issues.append(
                Issue(
                    path,
                    line_number(text, match.start()),
                    "Only the single store boundary may import Store.h. Use Actions for event dispatch facades, Selectors for reads, and Thunks/Listeners for workflows.",
                )
            )
    return issues


def check_store_access(path: Path, text: str, role: str, rel: Path) -> list[Issue]:
    issues: list[Issue] = []
    if "Store::GetStore(" in text and rel not in STORE_ACCESS_ALLOWLIST:
        issues.append(
            Issue(
                path,
                line_number(text, text.index("Store::GetStore(")),
                "Only the single store boundary may call Store::GetStore; route through Actions, Selectors, Thunks, or Listeners.",
            )
        )

    if re.search(r"(?<!RuntimeDispatch::)\bDispatch\s*\(|\.dispatch\s*\(|Api\.dispatch\b", text):
        if role not in RAW_DISPATCH_ALLOW_ROLES:
            issues.append(Issue(path, 1, f"{role} files must not dispatch; use event-style Actions, imperative Thunks, or reactive Listeners."))

    if re.search(r"\.getState\s*\(|Api\.getState\b", text):
        if role not in RAW_GET_STATE_ALLOW_ROLES and rel != Path("Systems/Selectors.cpp"):
            issues.append(Issue(path, 1, f"{role} files must not call getState; expose derived reads through Selectors or use getState inside Thunks/Listeners."))

    return issues


def check_rtk_factory_placement(path: Path, text: str, role: str) -> list[Issue]:
    return [
        Issue(path, line_number(text, match.start()), message)
        for pattern, allowed_roles, message in RTK_FACTORY_RULES
        for match in [pattern.search(text)]
        if match and role not in allowed_roles
    ]


def check_pure_roles(path: Path, text: str, role: str) -> list[Issue]:
    if role not in {"slice", "selectors", "types"}:
        return []
    return [
        Issue(path, line_number(text, match.start()), message)
        for pattern, message in REDUCER_SIDE_EFFECT_PATTERNS
        for match in [pattern.search(text)]
        if match
    ]


def check_rtk_query(path: Path, text: str) -> list[Issue]:
    if not RTK_QUERY_PATTERN.search(text) or RTK_QUERY_LIFECYCLE_PATTERN.search(text):
        return []
    return [
        Issue(
            path,
            first_line_for_pattern(text, RTK_QUERY_PATTERN),
            "Soft RTK Query upgrade: createApi/injectEndpoints boundaries should normally declare tags or endpoint lifecycles so cache invalidation and optimistic updates stay coupled to requests.",
            "warning",
        )
    ]


def check_store_boundary(project_root: Path) -> list[Issue]:
    findings: list[Issue] = []
    store_paths = [
        project_root / "Source/Store.h",
        project_root / "Source/Store.cpp",
    ]
    existing_store_paths = [path for path in store_paths if path.exists()]
    combined_store_text = "\n".join(
        path.read_text(encoding="utf-8", errors="replace")
        for path in existing_store_paths
    )

    for path in existing_store_paths:
        text = path.read_text(encoding="utf-8", errors="replace")
        for pattern, message in STORE_FORBIDDEN_PATTERN_RULES:
            match = pattern.search(text)
            if match:
                findings.append(Issue(path, line_number(text, match.start()), message))

    if existing_store_paths and not STORE_CONFIGURE_PATTERN.search(combined_store_text):
        findings.append(
            Issue(
                existing_store_paths[0],
                1,
                "Soft RTK upgrade: the store boundary should be visibly configureStore-style so default middleware/dev checks remain the baseline.",
                "warning",
            )
        )

    return findings


def check_file(path: Path, features_root: Path) -> list[Issue]:
    rel = path.relative_to(features_root)
    text = path.read_text(encoding="utf-8", errors="replace")
    role = role_for_stem(path.stem)
    issues: list[Issue] = []

    if role is None:
        issues.append(Issue(path, 1, role_name_guidance(path.stem)))
        role = "unknown"

    issues.extend(check_imports(path, text, role, rel))
    issues.extend(check_store_access(path, text, role, rel))
    issues.extend(check_rtk_factory_placement(path, text, role))
    issues.extend(check_pure_roles(path, text, role))
    issues.extend(check_rtk_query(path, text))

    role_module = ROLE_MODULES.get(role)
    if role_module:
        issues.extend(role_module.check(path, text))

    return issues


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "paths",
        nargs="*",
        type=Path,
        default=[Path("Source/Features")],
        help="Feature files or directories to scan.",
    )
    parser.add_argument(
        "--warnings-as-errors",
        action="store_true",
        help=(
            "Promote soft RTK upgrade findings to errors. "
            f"Can also be enabled with {WARNING_AS_ERROR_ENV}=1."
        ),
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    project_root = Path.cwd().resolve()
    features_root = (project_root / "Source/Features").resolve()
    issues: list[Issue] = []

    for path in iter_source_files(args.paths):
        resolved = path.resolve()
        if features_root not in resolved.parents and resolved != features_root:
            continue
        issues.extend(check_file(resolved, features_root))

    issues.extend(check_store_boundary(project_root))

    warnings_as_errors = args.warnings_as_errors or os.environ.get(
        WARNING_AS_ERROR_ENV
    ) in WARNING_TRUE_VALUES
    errors = [
        issue
        for issue in issues
        if issue.severity == "error" or (warnings_as_errors and issue.severity == "warning")
    ]
    warnings = [
        issue
        for issue in issues
        if issue.severity == "warning" and not warnings_as_errors
    ]

    if errors:
        print(f"RTK feature boundary guard failed: {len(errors)} issue(s).")
        for issue in errors:
            display = issue.path.relative_to(project_root)
            print(f"{display}:{issue.line}: {issue.message}")
        if warnings:
            print(f"\nSoft RTK upgrade warning(s): {len(warnings)}")
            for issue in warnings:
                display = issue.path.relative_to(project_root)
                print(f"{display}:{issue.line}: {issue.message}")
        print(
            "\nRole contract: Types are inert data; Slice owns createSlice, "
            "initial state, and pure reducer transitions; Selectors derive "
            "state; Thunks/Listeners own side effects; Adapters translate "
            "JSON/UE/ECS data; Actions expose event-style creators/facades."
        )
        return 1

    if warnings:
        print(f"RTK feature boundary guard passed with {len(warnings)} soft warning(s).")
        for issue in warnings:
            display = issue.path.relative_to(project_root)
            print(f"{display}:{issue.line}: {issue.message}")
        return 0

    print("RTK feature boundary guard passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
