#!/usr/bin/env python3
"""Enforce domain and subdomain ownership in file paths.

The script derives boundaries from the repository shape instead of a copied
domain list:

* source files use folders for domain tokens and exact role filenames;
* view files use subject folders and a role filename;
* flat data filenames move repeated prefix groups into folders.
"""

from __future__ import annotations

import argparse
import re
import sys
from collections import Counter
from dataclasses import dataclass
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
SOURCE_FEATURES_ROOT = PROJECT_ROOT / "Source" / "Features"
SOURCE_VIEWS_ROOT = PROJECT_ROOT / "Source" / "Views"
CONTENT_DATA_ROOT = PROJECT_ROOT / "Content" / "Data"

SOURCE_SUFFIXES = {".cpp", ".h", ".hpp"}
TEXT_SUFFIXES = {
    ".bat",
    ".build",
    ".cpp",
    ".cs",
    ".h",
    ".hpp",
    ".ini",
    ".json",
    ".md",
    ".ps1",
    ".py",
    ".sh",
    ".txt",
    ".uproject",
    ".uplugin",
    ".xml",
    ".yaml",
    ".yml",
}

CANONICAL_SOURCE_ROLES = {
    ("Actions",),
    ("Adapters",),
    ("Listeners",),
    ("Selectors",),
    ("Slice",),
    ("Thunks",),
    ("Types",),
}

SOURCE_ROLE_SUFFIXES = (
    *tuple(sorted(CANONICAL_SOURCE_ROLES)),
)

IGNORED_PARTS = {
    ".git",
    "Binaries",
    "DerivedDataCache",
    "Intermediate",
    "Saved",
}

TOKEN_ALIASES = {
    "ais": "ai",
    "bots": "bot",
    "buildings": "building",
    "characters": "character",
    "components": "component",
    "diagnostics": "diagnostic",
    "entities": "entity",
    "environments": "environment",
    "factories": "factory",
    "goals": "goal",
    "horses": "horse",
    "landmarks": "landmark",
    "lods": "lod",
    "palettes": "palette",
    "reducers": "reducer",
    "selectors": "selector",
    "settings": "setting",
    "systems": "system",
    "thunks": "thunk",
    "townspeople": "townsperson",
    "types": "type",
    "views": "view",
}

ACRONYM_TOKENS = {
    "AI",
    "CSV",
    "ECS",
    "FPS",
    "LOD",
    "LODs",
    "NPC",
    "OSM",
    "PS3",
    "RTK",
    "UI",
}

DATA_PREFIX_MIN_COUNT = 4
VIEW_QUALIFIER_MIN_COUNT = 3


@dataclass(frozen=True)
class BoundaryViolation:
    source: Path
    expected: Path
    reason: str


@dataclass(frozen=True)
class DataName:
    source: Path
    root: Path
    tokens: tuple[str, ...]


@dataclass(frozen=True)
class DataNamingContext:
    prefix_counts: Counter[tuple[str, ...]]
    compound_roots: frozenset[tuple[Path, tuple[str, str]]]


def display(path: Path) -> str:
    return path.relative_to(PROJECT_ROOT).as_posix()


def has_ignored_part(path: Path) -> bool:
    return any(part in IGNORED_PARTS for part in path.parts)


def camel_tokens(value: str) -> list[str]:
    if not value:
        return []
    chunks = re.findall(
        r"[A-Z]+(?=[A-Z][a-z]|[0-9]|\b)|[A-Z]?[a-z]+|[0-9]+", value
    )
    return chunks or [value]


def normalize_token(value: str) -> str:
    normalized = value.replace("-", "_").lower()
    return TOKEN_ALIASES.get(normalized, normalized)


def normalized_tokens(tokens: list[str] | tuple[str, ...]) -> list[str]:
    return [normalize_token(token) for token in tokens]


def folder_tokens(part: str) -> list[str]:
    tokens: list[str] = []
    for piece in re.split(r"[_\-\s]+", part):
        tokens.extend(camel_tokens(piece))
    return tokens


def role_file_name(tokens: list[str], suffix: str) -> str:
    return "".join(tokens) + suffix


def source_folder_name(token: str) -> str:
    return token if token in ACRONYM_TOKENS else token


def split_source_role(tokens: list[str]) -> tuple[list[str], list[str]]:
    for role in SOURCE_ROLE_SUFFIXES:
        if len(tokens) >= len(role) and tuple(tokens[-len(role) :]) == role:
            return tokens[: -len(role)], list(role)
    return tokens, []


def source_ancestor_tokens(path: Path) -> set[str]:
    rel = path.relative_to(SOURCE_FEATURES_ROOT)
    tokens: list[str] = []
    for part in rel.parent.parts:
        tokens.extend(folder_tokens(part))
    return set(normalized_tokens(tokens))


def source_violation_for(path: Path) -> BoundaryViolation | None:
    if path.suffix not in SOURCE_SUFFIXES:
        return None
    if not path.is_relative_to(SOURCE_FEATURES_ROOT):
        return None

    tokens = camel_tokens(path.stem)
    prefix_tokens, role_tokens = split_source_role(tokens)
    if not role_tokens:
        return None

    ancestor_tokens = source_ancestor_tokens(path)
    repeated_prefix = [
        token for token in prefix_tokens if normalize_token(token) in ancestor_tokens
    ]
    new_domain_tokens = [
        token for token in prefix_tokens if normalize_token(token) not in ancestor_tokens
    ]
    has_compound_leaf = prefix_tokens or role_file_name(role_tokens, path.suffix) != path.name
    if not repeated_prefix and not has_compound_leaf:
        return None

    expected_parent = path.parent
    for token in new_domain_tokens:
        expected_parent /= source_folder_name(token)
    expected = expected_parent / role_file_name(role_tokens, path.suffix)
    if expected == path:
        return None

    return BoundaryViolation(
        source=path,
        expected=expected,
        reason="source filename repeats domain/subdomain tokens",
    )


def view_subject_tokens(path: Path) -> list[str]:
    tokens = camel_tokens(path.stem)
    if tokens and tokens[-1] == "View":
        return tokens[:-1]
    return []


def view_qualifier_tokens(paths: list[Path]) -> set[str]:
    counts: Counter[str] = Counter()
    seen_stems: set[str] = set()
    for path in paths:
        if path.suffix not in SOURCE_SUFFIXES or not path.is_relative_to(SOURCE_VIEWS_ROOT):
            continue
        if path.stem in seen_stems:
            continue
        seen_stems.add(path.stem)
        for token in view_subject_tokens(path):
            counts[normalize_token(token)] += 1
    return {
        token
        for token, count in counts.items()
        if count >= VIEW_QUALIFIER_MIN_COUNT
    }


def view_violation_for(
    path: Path, qualifier_tokens: set[str]
) -> BoundaryViolation | None:
    if path.suffix not in SOURCE_SUFFIXES:
        return None
    if not path.is_relative_to(SOURCE_VIEWS_ROOT):
        return None

    subject_tokens = [
        token
        for token in view_subject_tokens(path)
        if normalize_token(token) not in qualifier_tokens
    ]
    if not subject_tokens:
        return None

    expected_parent = SOURCE_VIEWS_ROOT
    for token in subject_tokens:
        expected_parent /= source_folder_name(token)
    expected = expected_parent / ("View" + path.suffix)
    if expected == path:
        return None

    return BoundaryViolation(
        source=path,
        expected=expected,
        reason="view filename repeats subject tokens",
    )


def data_root_for(path: Path) -> Path | None:
    if path.parent == CONTENT_DATA_ROOT:
        return CONTENT_DATA_ROOT
    source_data_root = CONTENT_DATA_ROOT / "Source"
    if path.parent == source_data_root:
        return source_data_root
    return None


def flat_json_data_names(paths: list[Path]) -> list[DataName]:
    names: list[DataName] = []
    for path in paths:
        if path.suffix != ".json" or not path.is_relative_to(CONTENT_DATA_ROOT):
            continue
        root = data_root_for(path)
        if root is None:
            continue
        tokens = tuple(token for token in path.stem.split("_") if token)
        if len(tokens) > 1:
            names.append(DataName(path, root, tokens))
    return names


def data_prefixes(tokens: tuple[str, ...]) -> list[tuple[str, ...]]:
    return [tokens[:index] for index in range(1, len(tokens))]


def data_naming_context(paths: list[Path]) -> DataNamingContext:
    names = flat_json_data_names(paths)
    first_token_files = {
        (root, path.stem)
        for path in paths
        if path.suffix == ".json"
        for root in [data_root_for(path)]
        if root is not None and path.parent == root
    }
    pair_counts: Counter[tuple[Path, tuple[str, str]]] = Counter()
    prefix_counts: Counter[tuple[str, ...]] = Counter()

    for name in names:
        if len(name.tokens) >= 2:
            pair_counts[(name.root, (name.tokens[0], name.tokens[1]))] += 1

    compound_roots = frozenset(
        pair
        for pair, count in pair_counts.items()
        if count >= DATA_PREFIX_MIN_COUNT and (pair[0], pair[1][0]) not in first_token_files
    )

    for name in names:
        tokens = normalized_data_tokens(name, compound_roots)
        prefix_counts.update(data_prefixes(tokens))

    return DataNamingContext(prefix_counts=prefix_counts, compound_roots=compound_roots)


def normalized_data_tokens(
    name: DataName, compound_roots: frozenset[tuple[Path, tuple[str, str]]]
) -> tuple[str, ...]:
    if len(name.tokens) >= 2 and (name.root, (name.tokens[0], name.tokens[1])) in compound_roots:
        return ("_".join(name.tokens[:2]), *name.tokens[2:])
    return name.tokens


def best_data_prefix(tokens: tuple[str, ...], context: DataNamingContext) -> tuple[str, ...]:
    candidates = [
        prefix
        for prefix in data_prefixes(tokens)
        if context.prefix_counts[prefix] >= DATA_PREFIX_MIN_COUNT
    ]
    if not candidates:
        if len(tokens) <= 2:
            return tokens[:1]
        return tokens[:-2]
    return max(candidates, key=len)


def data_violation_for(
    path: Path, context: DataNamingContext
) -> BoundaryViolation | None:
    if path.suffix != ".json" or not path.is_relative_to(CONTENT_DATA_ROOT):
        return None
    root = data_root_for(path)
    if root is None:
        return None

    tokens = tuple(token for token in path.stem.split("_") if token)
    if len(tokens) <= 1:
        return None

    name = DataName(path, root, tokens)
    normalized = normalized_data_tokens(name, context.compound_roots)
    prefix = best_data_prefix(normalized, context)
    leaf_tokens = normalized[len(prefix) :]
    if not prefix or not leaf_tokens:
        return None

    expected = root / Path(*prefix) / ("_".join(leaf_tokens) + ".json")
    if expected == path:
        return None

    return BoundaryViolation(
        source=path,
        expected=expected,
        reason="data filename repeats folderable prefix tokens",
    )


def iter_candidates(paths: list[Path]) -> list[Path]:
    candidates: list[Path] = []
    for path in paths:
        root = path if path.is_absolute() else PROJECT_ROOT / path
        if root.is_file():
            candidates.append(root)
        elif root.is_dir():
            candidates.extend(child for child in root.rglob("*") if child.is_file())
    return [path for path in candidates if not has_ignored_part(path)]


def find_violations(paths: list[Path]) -> list[BoundaryViolation]:
    candidates = iter_candidates(paths)
    qualifier_tokens = view_qualifier_tokens(candidates)
    data_context = data_naming_context(candidates)
    violations: list[BoundaryViolation] = []

    for path in candidates:
        view_violation = view_violation_for(path, qualifier_tokens)
        if view_violation is not None:
            violations.append(view_violation)
            continue

        source_violation = source_violation_for(path)
        if source_violation is not None:
            violations.append(source_violation)
            continue

        data_violation = data_violation_for(path, data_context)
        if data_violation is not None:
            violations.append(data_violation)

    return sorted(violations, key=lambda violation: display(violation.source))


def replacement_pairs(violations: list[BoundaryViolation]) -> list[tuple[str, str]]:
    pairs: list[tuple[str, str]] = []
    for violation in violations:
        old = violation.source.relative_to(PROJECT_ROOT).as_posix()
        new = violation.expected.relative_to(PROJECT_ROOT).as_posix()
        pairs.append((old, new))
        if old.startswith("Source/"):
            pairs.append((old.removeprefix("Source/"), new.removeprefix("Source/")))
        if old.startswith("Content/"):
            pairs.append((old.removeprefix("Content/"), new.removeprefix("Content/")))
        pairs.append((old.replace("/", "\\"), new.replace("/", "\\")))
        if old.startswith("Content/"):
            pairs.append(
                (
                    old.removeprefix("Content/").replace("/", "\\"),
                    new.removeprefix("Content/").replace("/", "\\"),
                )
            )
        if violation.source.suffix == ".h" and violation.expected.suffix == ".h":
            pairs.append(
                (
                    f'"{violation.source.stem}.generated.h"',
                    f'"{violation.expected.stem}.generated.h"',
                )
            )
    return sorted(set(pairs), key=lambda pair: len(pair[0]), reverse=True)


def iter_text_files() -> list[Path]:
    roots = [
        PROJECT_ROOT / "Content",
        PROJECT_ROOT / "DOCS",
        PROJECT_ROOT / "README.md",
        PROJECT_ROOT / "Scripts",
        PROJECT_ROOT / "Source",
    ]
    files: list[Path] = []
    for root in roots:
        if root.is_file():
            files.append(root)
        elif root.is_dir():
            files.extend(child for child in root.rglob("*") if child.is_file())
    return [
        path
        for path in files
        if path.suffix in TEXT_SUFFIXES and not has_ignored_part(path)
    ]


def rewrite_references(violations: list[BoundaryViolation]) -> int:
    pairs = replacement_pairs(violations)
    rewritten = 0
    for path in iter_text_files():
        try:
            original = path.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            continue
        updated = original
        for old, new in pairs:
            updated = updated.replace(old, new)
        if updated != original:
            path.write_text(updated, encoding="utf-8")
            rewritten += 1
    return rewritten


def apply_fixes(violations: list[BoundaryViolation]) -> int:
    expected_paths = [violation.expected for violation in violations]
    if len(expected_paths) != len(set(expected_paths)):
        print("domain boundary fix would create duplicate destination paths", file=sys.stderr)
        return 2

    for violation in violations:
        if violation.expected.exists() and violation.expected != violation.source:
            print(
                f"domain boundary fix blocked: {display(violation.expected)} already exists",
                file=sys.stderr,
            )
            return 2

    for violation in violations:
        violation.expected.parent.mkdir(parents=True, exist_ok=True)
        violation.source.rename(violation.expected)

    rewrite_references(violations)
    return 0


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "paths",
        nargs="*",
        type=Path,
        default=[SOURCE_FEATURES_ROOT, SOURCE_VIEWS_ROOT, CONTENT_DATA_ROOT],
        help="files or directories to scan",
    )
    parser.add_argument(
        "--fix",
        action="store_true",
        help="move violating files and rewrite textual references",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    violations = find_violations(args.paths)
    if not violations:
        print("Domain boundary check passed.")
        return 0

    for violation in violations:
        print(
            f"{display(violation.source)} -> {display(violation.expected)}"
            f" ({violation.reason})"
        )

    if args.fix:
        fixed = apply_fixes(violations)
        if fixed != 0:
            return fixed
        print(f"Moved {len(violations)} domain-boundary files.")
        return 0

    print(f"Domain boundary check failed: {len(violations)} violation(s).", file=sys.stderr)
    return 1


if __name__ == "__main__":
    raise SystemExit(main())
