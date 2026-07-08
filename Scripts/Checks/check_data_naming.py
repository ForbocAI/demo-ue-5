#!/usr/bin/env python3
"""Enforce ECS-shaped authored-data naming.

JSON paths read like ECS domain paths: broad catalog, concrete instance,
subdomain, then a small local leaf. The checker derives violations from path and
object shape so concrete instances and subdomains can move without a second
managed catalog in this script.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from dataclasses import dataclass
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
CONTENT_DATA_ROOT = PROJECT_ROOT / "Content" / "Data"
IGNORED_PARTS = {
    ".git",
    "Binaries",
    "DerivedDataCache",
    "Intermediate",
    "Saved",
    "__pycache__",
}
LARGE_JSON_LINE_COUNT = 500
WIDE_SECTION_COUNT = 4
ECS_SECTION_KEYS = {
    "capabilities",
    "component_schemas",
    "components",
    "domain_registry",
    "domains",
    "entities",
    "events",
    "resources",
    "systems",
}
INSTANCE_CATALOG_ATOM = "level"


@dataclass(frozen=True)
class Issue:
    path: Path
    message: str


def display(path: Path) -> str:
    return path.relative_to(PROJECT_ROOT).as_posix()


def has_ignored_part(path: Path) -> bool:
    return any(part in IGNORED_PARTS for part in path.parts)


def iter_json_files(roots: list[Path]) -> list[Path]:
    files: list[Path] = []
    for root in roots:
        path = root if root.is_absolute() else PROJECT_ROOT / root
        if path.is_file() and path.suffix == ".json":
            files.append(path)
        elif path.is_dir():
            files.extend(
                child
                for child in path.rglob("*.json")
                if child.is_file()
            )
    return sorted(path for path in files if not has_ignored_part(path))


def normalized(value: str) -> str:
    return re.sub(r"[^a-z0-9]+", "_", value.lower()).strip("_")


def plural_catalog(atom: str) -> str:
    if atom.endswith("y"):
        return atom[:-1] + "ies"
    if atom.endswith("s"):
        return atom
    return atom + "s"


def path_tokens(path: Path) -> list[str]:
    if not path.is_relative_to(CONTENT_DATA_ROOT):
        return []
    relative = path.relative_to(CONTENT_DATA_ROOT)
    tokens: list[str] = []
    for part in relative.with_suffix("").parts:
        tokens.extend(token for token in re.split(r"[_\-/\s]+", part) if token)
    return [normalized(token) for token in tokens]


def infer_location_roots(files: list[Path]) -> frozenset[str]:
    """Find concrete location folders from the repo shape.

    A location root is inferred when its first child is the level catalog atom.
    This keeps the rule data-driven for concrete authored domain names.
    """
    roots: set[str] = set()
    for path in files:
        if not path.is_relative_to(CONTENT_DATA_ROOT):
            continue
        relative = path.relative_to(CONTENT_DATA_ROOT)
        if len(relative.parts) >= 3 and relative.parts[1] == INSTANCE_CATALOG_ATOM:
            roots.add(relative.parts[0])
    return frozenset(roots)


def is_json_path(value: object) -> bool:
    return isinstance(value, str) and value.endswith(".json")


def is_json_path_manifest(value: object) -> bool:
    if isinstance(value, dict) and value:
        return all(is_json_path_manifest(child) for child in value.values())
    if isinstance(value, list) and value:
        return all(is_json_path(child) for child in value)
    return is_json_path(value)


def catalog_instance_expected_path(
    path: Path, location_roots: frozenset[str]
) -> Path | None:
    """Lift `name/level/...` and `name/...` into `levels/name/...`.

    ECS domain paths are ordered from catalog to concrete instance. The literal
    location name is the instance; once the repo shape identifies a location
    root, every file under it belongs below the plural catalog root.
    """
    if not path.is_relative_to(CONTENT_DATA_ROOT):
        return None
    relative = path.relative_to(CONTENT_DATA_ROOT)
    if len(relative.parts) < 2:
        return None
    instance, *rest = relative.parts
    if instance not in location_roots:
        return None
    if rest and rest[0] == INSTANCE_CATALOG_ATOM:
        rest = rest[1:]
    if not rest:
        return None
    return (
        CONTENT_DATA_ROOT
        / plural_catalog(INSTANCE_CATALOG_ATOM)
        / instance
        / Path(*rest)
    )


def leaf_split_path(path: Path) -> Path | None:
    tokens = [token for token in path.stem.split("_") if token]
    if len(tokens) <= 1:
        return None
    if path.parent.name == tokens[0]:
        return path.parent / ("_".join(tokens[1:]) + path.suffix)
    return path.parent / tokens[0] / ("_".join(tokens[1:]) + path.suffix)


def compound_leaf_issue(path: Path, location_roots: frozenset[str]) -> Issue | None:
    if not path.is_relative_to(CONTENT_DATA_ROOT):
        return None
    ordered_path = catalog_instance_expected_path(path, location_roots) or path
    expected = leaf_split_path(ordered_path)
    if expected is None:
        return None
    if expected == path:
        return None
    return Issue(
        path,
        f"JSON filename repeats folderable ECS path tokens. Expected shape: {display(expected)}",
    )


def catalog_instance_order_issue(
    path: Path, location_roots: frozenset[str]
) -> Issue | None:
    expected = catalog_instance_expected_path(path, location_roots)
    if expected is None or expected == path:
        return None
    return Issue(
        path,
        f"ECS data path puts the concrete location before the catalog. Expected catalog/instance shape: {display(expected)}",
    )


def line_count(path: Path) -> int:
    return path.read_text(encoding="utf-8", errors="replace").count("\n") + 1


def load_json(path: Path) -> object | None:
    try:
        return json.loads(path.read_text(encoding="utf-8"))
    except json.JSONDecodeError:
        return None


def local_split_base(
    path: Path, location_roots: frozenset[str], root_key: str | None = None
) -> Path:
    ordered = catalog_instance_expected_path(path, location_roots) or path
    base = ordered.with_suffix("")
    if root_key is not None and normalized(base.name) != normalized(root_key):
        return base / root_key
    return base


def split_examples(base: Path, keys: list[str]) -> str:
    return ", ".join(display(base / f"{key}.json") for key in keys[:4])


def root_key_rewrap_issue(
    path: Path, value: object, location_roots: frozenset[str]
) -> Issue | None:
    if not path.is_relative_to(CONTENT_DATA_ROOT):
        return None
    if not isinstance(value, dict) or len(value) != 1 or is_json_path_manifest(value):
        return None

    root_key, root_value = next(iter(value.items()))
    root_name = normalized(root_key)
    repeats_path = root_name == normalized(path.stem) or root_name in path_tokens(path)
    if not repeats_path:
        return None

    base = local_split_base(path, location_roots, root_key)
    if isinstance(root_value, dict):
        section_keys = [
            key for key, child in root_value.items() if isinstance(child, (dict, list))
        ]
        if not section_keys:
            return None
        detail = f"Split local sections into ECS leaf files such as {split_examples(base, section_keys)}."
    elif isinstance(root_value, list):
        detail = (
            "Put the array directly at the leaf or split repeated row groups "
            f"under {display(base)}/."
        )
    else:
        return None
    return Issue(
        path,
        f"JSON root `{root_key}` re-wraps a domain already named by the path. {detail}",
    )


def ecs_wide_section_issue(
    path: Path, value: object, location_roots: frozenset[str]
) -> Issue | None:
    if not path.is_relative_to(CONTENT_DATA_ROOT):
        return None
    if not isinstance(value, dict) or is_json_path_manifest(value):
        return None

    ecs_keys = [
        key
        for key, child in value.items()
        if normalized(key) in ECS_SECTION_KEYS and isinstance(child, (dict, list))
    ]
    if len(ecs_keys) < 2:
        return None

    base = local_split_base(path, location_roots)
    return Issue(
        path,
        f"JSON mixes ECS registry sections ({', '.join(ecs_keys)}). Keep entity/component/system/resource/event declarations in folder-local files such as {split_examples(base, ecs_keys)} and compose them through one ECS registry fold.",
    )


def location_scoped_definition_issue(
    path: Path, value: object, location_roots: frozenset[str]
) -> Issue | None:
    if not path.is_relative_to(CONTENT_DATA_ROOT):
        return None
    relative = path.relative_to(CONTENT_DATA_ROOT)
    if len(relative.parts) < 2 or relative.parts[0] not in location_roots:
        return None
    if not isinstance(value, dict) or len(value) != 1:
        return None
    rows = next(iter(value.values()))
    if not isinstance(rows, list) or not any(isinstance(row, dict) for row in rows):
        return None
    return Issue(
        path,
        "Location-scoped row data holds instance references and overrides, not wide reusable definition bags. Split reusable definitions into shared catalogs and keep this location leaf focused on placement/override rows.",
    )


def large_section_issue(
    path: Path, location_roots: frozenset[str], value: object
) -> Issue | None:
    if not path.is_relative_to(CONTENT_DATA_ROOT):
        return None
    lines = line_count(path)
    if lines < LARGE_JSON_LINE_COUNT:
        return None
    if not isinstance(value, dict):
        return None
    if is_json_path_manifest(value):
        return None

    if len(value) == 1:
        root_key, root_value = next(iter(value.items()))
        if isinstance(root_value, dict):
            array_keys = [
                key
                for key, child in root_value.items()
                if isinstance(child, list)
            ]
            if array_keys:
                examples = split_examples(
                    local_split_base(path, location_roots, root_key), array_keys
                )
                return Issue(
                    path,
                    f"Large JSON root `{root_key}` contains split-ready ECS arrays ({', '.join(array_keys)}). Move them into subdomain files such as {examples}.",
                )

    object_sections = [
        key
        for key, child in value.items()
        if isinstance(child, (dict, list))
    ]
    if len(object_sections) >= WIDE_SECTION_COUNT:
        examples = split_examples(local_split_base(path, location_roots), object_sections)
        return Issue(
            path,
            f"Large JSON file mixes {len(object_sections)} data sections. Split sections into folder-local files such as {examples} and compose them through ECS/adapter folds.",
        )
    return None


def find_issues(roots: list[Path]) -> list[Issue]:
    issues: list[Issue] = []
    files = iter_json_files(roots)
    location_roots = infer_location_roots(files)
    for path in files:
        value = load_json(path)
        for issue in (
            catalog_instance_order_issue(path, location_roots),
            compound_leaf_issue(path, location_roots),
            root_key_rewrap_issue(path, value, location_roots),
            ecs_wide_section_issue(path, value, location_roots),
            location_scoped_definition_issue(path, value, location_roots),
            large_section_issue(path, location_roots, value),
        ):
            if issue is not None:
                issues.append(issue)
    return sorted(issues, key=lambda issue: display(issue.path))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("paths", nargs="*", type=Path, default=[Path("Content/Data")])
    args = parser.parse_args()

    issues = find_issues(args.paths)
    for issue in issues:
        print(f"{display(issue.path)}: {issue.message}")
    if issues:
        print("")
        print(
            "Authored-data naming check failed. Use ECS-shaped folder segments, infer catalog/instance order from the repo shape, split reusable definitions from location overrides, and compose leaf JSON files through manifest/adapter folds."
        )
        return 1
    print("Authored-data naming check passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
