#!/usr/bin/env python3
"""Domain/subdomain ownership guard for file paths.

Boundaries are derived from the repository shape, not a managed list:

* source files put domain tokens in folders and keep an exact role leaf;
* view files put subject tokens in folders and keep the View leaf;
* data files fold repeated prefix groups (and ECS section/kind prefixes) into
  folders;
* a folder never repeats its parent.

This guard is report-only. It computes the constructive target path for every
violation and tells the developer the move to make -- it never renames files or
rewrites references itself.
"""

from __future__ import annotations

import argparse
from collections import Counter
from dataclasses import dataclass
from pathlib import Path
import sys

# The ECS engine (check_ecs) lives one level up in Checks/; put it on the path.
sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from check_ecs import (
    Finding,
    Rule,
    Severity,
    apply_suppressions,
    camel_tokens,
    ecs_domain_kinds,
    ecs_section_keys,
    explain,
    folder_tokens,
    format_json,
    format_sarif,
    format_text,
    iter_files,
    normalize_token,
    normalized_tokens,
    register,
    rel,
    PROJECT_ROOT,
)


SOURCE_FEATURES_ROOT = PROJECT_ROOT / "Source" / "Features"
SOURCE_VIEWS_ROOT = PROJECT_ROOT / "Source" / "Views"
CONTENT_DATA_ROOT = PROJECT_ROOT / "Content" / "Data"

SOURCE_SUFFIXES = {".cpp", ".h", ".hpp"}
CANONICAL_SOURCE_ROLES = (
    ("Actions",),
    ("Adapters",),
    ("Listeners",),
    ("Selectors",),
    ("Slice",),
    ("Thunks",),
    ("Types",),
)
DATA_PREFIX_MIN_COUNT = 4
VIEW_QUALIFIER_MIN_COUNT = 3

_SELF_MOVE_NOTE = (
    " Rename/move the file to that path and update its references yourself; this "
    "guard reports the target and never edits files."
)

SRC_REPEAT = register(
    Rule(
        id="DOMAIN-SRC-001",
        severity=Severity.HIGH,
        summary="source filename repeats domain/subdomain tokens already in its folders",
        guidance="Move the domain/subdomain words into folders and keep the exact role leaf (Rendering/Sky/Thunks.h)." + _SELF_MOVE_NOTE,
        skill="ecs.hpp doctrine: domain words live in folders; the leaf is a small role name",
    )
)

VIEW_REPEAT = register(
    Rule(
        id="DOMAIN-VIEW-001",
        severity=Severity.HIGH,
        summary="view filename repeats subject tokens shared across views",
        guidance="Move the shared subject word into a folder and keep the leaf View (Player/Controller/View.cpp)." + _SELF_MOVE_NOTE,
        skill="ecs.hpp doctrine: subject in folders, View leaf",
    )
)

DATA_REPEAT = register(
    Rule(
        id="DOMAIN-DATA-001",
        severity=Severity.HIGH,
        summary="data filename repeats folderable prefix tokens",
        guidance="Fold the repeated prefix into a folder (french_gulch/roads/residential.json)." + _SELF_MOVE_NOTE,
        skill="check_data_naming doctrine: catalog/instance/subdomain/leaf",
    )
)

DATA_ECS_PREFIX = register(
    Rule(
        id="DOMAIN-DATA-002",
        severity=Severity.MEDIUM,
        summary="flat data filename leads with an ECS section/kind that should be its folder",
        guidance="Put the file under a folder named for the ECS registry section or domain kind (components/..., systems/...) so authored data mirrors the ecs.hpp registry." + _SELF_MOVE_NOTE,
        skill="ecs.hpp: EDomainKind + FDomainRegistry sections",
    )
)

FOLDER_REDUNDANT = register(
    Rule(
        id="DOMAIN-FOLDER-001",
        severity=Severity.MEDIUM,
        summary="folder segment repeats its parent segment",
        guidance="Collapse the redundant nesting; each path segment should add a new domain word, not repeat its parent." + _SELF_MOVE_NOTE,
        skill="ecs.hpp doctrine: every path segment is a distinct domain step",
    )
)


def _move(path: Path, expected: Path, rule: Rule) -> Finding:
    return Finding(path, 1, rule.id, rule.severity, f"{rule.summary}; expected `{rel(expected)}`")


# --- Source leaves ---------------------------------------------------------

def _split_source_role(tokens: list[str]) -> tuple[list[str], list[str]]:
    for role in CANONICAL_SOURCE_ROLES:
        if len(tokens) >= len(role) and tuple(tokens[-len(role):]) == role:
            return tokens[: -len(role)], list(role)
    return tokens, []


def _source_ancestor_tokens(path: Path) -> set[str]:
    tokens: list[str] = []
    for part in path.relative_to(SOURCE_FEATURES_ROOT).parent.parts:
        tokens.extend(folder_tokens(part))
    return set(normalized_tokens(tokens))


def source_finding(path: Path) -> Finding | None:
    if path.suffix not in SOURCE_SUFFIXES or not path.is_relative_to(SOURCE_FEATURES_ROOT):
        return None
    tokens = camel_tokens(path.stem)
    prefix_tokens, role_tokens = _split_source_role(tokens)
    if not role_tokens:
        return None
    ancestor = _source_ancestor_tokens(path)
    repeated = [t for t in prefix_tokens if normalize_token(t) in ancestor]
    new_tokens = [t for t in prefix_tokens if normalize_token(t) not in ancestor]
    has_compound = bool(prefix_tokens) or ("".join(role_tokens) + path.suffix) != path.name
    if not repeated and not has_compound:
        return None
    expected_parent = path.parent
    for token in new_tokens:
        expected_parent /= token
    expected = expected_parent / ("".join(role_tokens) + path.suffix)
    return None if expected == path else _move(path, expected, SRC_REPEAT)


# --- View subjects ---------------------------------------------------------

def _view_subject_tokens(path: Path) -> list[str]:
    tokens = camel_tokens(path.stem)
    return tokens[:-1] if tokens and tokens[-1] == "View" else []


def view_qualifier_tokens(paths: list[Path]) -> set[str]:
    counts: Counter[str] = Counter()
    seen: set[str] = set()
    for path in paths:
        if path.suffix not in SOURCE_SUFFIXES or not path.is_relative_to(SOURCE_VIEWS_ROOT):
            continue
        if path.stem in seen:
            continue
        seen.add(path.stem)
        for token in _view_subject_tokens(path):
            counts[normalize_token(token)] += 1
    return {token for token, count in counts.items() if count >= VIEW_QUALIFIER_MIN_COUNT}


def view_finding(path: Path, qualifiers: set[str]) -> Finding | None:
    if path.suffix not in SOURCE_SUFFIXES or not path.is_relative_to(SOURCE_VIEWS_ROOT):
        return None
    subject = [t for t in _view_subject_tokens(path) if normalize_token(t) not in qualifiers]
    if not subject:
        return None
    expected_parent = SOURCE_VIEWS_ROOT
    for token in subject:
        expected_parent /= token
    expected = expected_parent / ("View" + path.suffix)
    return None if expected == path else _move(path, expected, VIEW_REPEAT)


# --- Data filenames --------------------------------------------------------

@dataclass(frozen=True)
class DataContext:
    prefix_counts: Counter[tuple[str, ...]]
    compound_roots: frozenset[tuple[Path, tuple[str, str]]]


def _data_root_for(path: Path) -> Path | None:
    if path.parent == CONTENT_DATA_ROOT:
        return CONTENT_DATA_ROOT
    source_root = CONTENT_DATA_ROOT / "Source"
    return source_root if path.parent == source_root else None


def _data_names(paths: list[Path]) -> list[tuple[Path, Path, tuple[str, ...]]]:
    names = []
    for path in paths:
        if path.suffix != ".json" or not path.is_relative_to(CONTENT_DATA_ROOT):
            continue
        root = _data_root_for(path)
        if root is None:
            continue
        tokens = tuple(token for token in path.stem.split("_") if token)
        if len(tokens) > 1:
            names.append((path, root, tokens))
    return names


def _data_prefixes(tokens: tuple[str, ...]) -> list[tuple[str, ...]]:
    return [tokens[:index] for index in range(1, len(tokens))]


def data_context(paths: list[Path]) -> DataContext:
    names = _data_names(paths)
    first_token_files = {
        (root, path.stem)
        for path in paths
        if path.suffix == ".json"
        for root in [_data_root_for(path)]
        if root is not None and path.parent == root
    }
    pair_counts: Counter[tuple[Path, tuple[str, str]]] = Counter()
    for _path, root, tokens in names:
        if len(tokens) >= 2:
            pair_counts[(root, (tokens[0], tokens[1]))] += 1
    compound_roots = frozenset(
        pair
        for pair, count in pair_counts.items()
        if count >= DATA_PREFIX_MIN_COUNT and (pair[0], pair[1][0]) not in first_token_files
    )
    prefix_counts: Counter[tuple[str, ...]] = Counter()
    for _path, root, tokens in names:
        prefix_counts.update(_data_prefixes(_normalized_data_tokens(root, tokens, compound_roots)))
    return DataContext(prefix_counts=prefix_counts, compound_roots=compound_roots)


def _normalized_data_tokens(
    root: Path, tokens: tuple[str, ...], compound_roots: frozenset[tuple[Path, tuple[str, str]]]
) -> tuple[str, ...]:
    if len(tokens) >= 2 and (root, (tokens[0], tokens[1])) in compound_roots:
        return ("_".join(tokens[:2]), *tokens[2:])
    return tokens


def _best_data_prefix(tokens: tuple[str, ...], context: DataContext) -> tuple[str, ...]:
    candidates = [
        prefix for prefix in _data_prefixes(tokens)
        if context.prefix_counts[prefix] >= DATA_PREFIX_MIN_COUNT
    ]
    if candidates:
        return max(candidates, key=len)
    return tokens[:1] if len(tokens) <= 2 else tokens[:-2]


def data_ecs_finding(path: Path) -> Finding | None:
    """A flat data file whose leading token is an ECS section/kind belongs in
    that folder regardless of how many siblings share the prefix."""
    if path.suffix != ".json":
        return None
    root = _data_root_for(path)
    if root is None:
        return None
    tokens = tuple(token for token in path.stem.split("_") if token)
    if len(tokens) <= 1:
        return None
    ecs_words = ecs_section_keys() | {normalize_token(k) for k in ecs_domain_kinds()}
    if normalize_token(tokens[0]) not in ecs_words and tokens[0] not in ecs_words:
        return None
    expected = root / tokens[0] / ("_".join(tokens[1:]) + ".json")
    return None if expected == path else _move(path, expected, DATA_ECS_PREFIX)


def data_freq_finding(path: Path, context: DataContext) -> Finding | None:
    if path.suffix != ".json":
        return None
    root = _data_root_for(path)
    if root is None:
        return None
    tokens = tuple(token for token in path.stem.split("_") if token)
    if len(tokens) <= 1:
        return None
    normalized = _normalized_data_tokens(root, tokens, context.compound_roots)
    prefix = _best_data_prefix(normalized, context)
    leaf = normalized[len(prefix):]
    if not prefix or not leaf:
        return None
    expected = root / Path(*prefix) / ("_".join(leaf) + ".json")
    return None if expected == path else _move(path, expected, DATA_REPEAT)


# --- Redundant folders -----------------------------------------------------

def folder_redundancy_findings(paths: list[Path]) -> list[Finding]:
    seen: set[Path] = set()
    findings: list[Finding] = []
    for path in paths:
        parts = path.parent.parts
        for index in range(1, len(parts)):
            if normalize_token(parts[index]) == normalize_token(parts[index - 1]):
                folder = Path(*parts[: index + 1])
                if folder in seen:
                    continue
                seen.add(folder)
                collapsed = Path(*parts[:index], *parts[index + 1:])
                findings.append(
                    Finding(
                        folder,
                        1,
                        FOLDER_REDUNDANT.id,
                        FOLDER_REDUNDANT.severity,
                        f"{FOLDER_REDUNDANT.summary} (`{parts[index]}`); expected `{rel(collapsed)}`",
                    )
                )
    return findings


# --- Runner ----------------------------------------------------------------

def find_findings(paths: list[Path]) -> list[Finding]:
    candidates = iter_files(paths)
    qualifiers = view_qualifier_tokens(candidates)
    context = data_context(candidates)
    findings: list[Finding] = []
    for path in candidates:
        finding = (
            view_finding(path, qualifiers)
            or source_finding(path)
            or data_ecs_finding(path)
            or data_freq_finding(path, context)
        )
        if finding is not None:
            findings.append(finding)
    findings += folder_redundancy_findings(candidates)
    return findings


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "paths",
        nargs="*",
        type=Path,
        default=[SOURCE_FEATURES_ROOT, SOURCE_VIEWS_ROOT, CONTENT_DATA_ROOT],
        help="files or directories to scan",
    )
    parser.add_argument("--format", choices=("text", "json", "sarif"), default="text")
    parser.add_argument("--explain", nargs="?", const="", metavar="RULE-ID")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.explain is not None:
        print(explain(args.explain or None))
        return 0

    findings = find_findings(args.paths)
    guard = "Domain boundary guard"
    if args.format == "json":
        print(format_json(findings, PROJECT_ROOT))
    elif args.format == "sarif":
        print(format_sarif(findings, PROJECT_ROOT, guard))
    elif findings:
        print(format_text(findings, PROJECT_ROOT, guard, 0))
    else:
        print(f"{guard} passed.")
    return 1 if findings else 0


if __name__ == "__main__":
    raise SystemExit(main())
