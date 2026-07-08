#!/usr/bin/env python3
"""ECS checks engine -- the sibling of check_redux.

The ECS naming/domain guards build on this engine the way the RTK role guards
build on check_redux. The two engines are peers: they mirror the same
finding model (Severity, Finding, Rule registry, text/json/sarif emitters,
governed suppression, ``--explain``) but neither imports the other, matching the
fp/rtk/ecs tri-core parity rule that ECS never depends on RTK.

On top of the mirrored finding model this engine owns ECS-specific machinery: one
canonical tokenizer + path helpers shared by the naming guards, and the ECS
vocabulary derived from Source/Core/ecs.hpp so the naming doctrine tracks the ECS
core instead of a hand-maintained copy.
"""

from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from functools import lru_cache
from pathlib import Path
import re


class Severity(str, Enum):
    CRITICAL = "critical"
    HIGH = "high"
    MEDIUM = "medium"
    LOW = "low"


SEVERITY_ORDER = {
    Severity.CRITICAL: 0,
    Severity.HIGH: 1,
    Severity.MEDIUM: 2,
    Severity.LOW: 3,
}

SARIF_LEVEL = {
    Severity.CRITICAL: "error",
    Severity.HIGH: "error",
    Severity.MEDIUM: "error",
    Severity.LOW: "error",
}


@dataclass(frozen=True)
class Rule:
    """A guard rule: stable id, severity, and the skill/doctrine it enforces."""

    id: str
    severity: Severity
    summary: str
    guidance: str
    skill: str
    roles: frozenset[str] = frozenset()  # empty => global / any scope


RULES: dict[str, Rule] = {}


def register(rule: Rule) -> Rule:
    if rule.id in RULES and RULES[rule.id] is not rule:
        raise ValueError(f"duplicate rule id: {rule.id}")
    RULES[rule.id] = rule
    return rule


@dataclass(frozen=True)
class Finding:
    path: Path
    line: int
    rule_id: str
    severity: Severity
    message: str
    column: int = 1

    @property
    def guidance(self) -> str:
        rule = RULES.get(self.rule_id)
        return rule.guidance if rule else ""


def line_number(text: str, index: int) -> int:
    return text.count("\n", 0, index) + 1


def column_number(text: str, index: int) -> int:
    return index - (text.rfind("\n", 0, index) + 1) + 1


# --- Suppressions ----------------------------------------------------------

def apply_suppressions(
    findings: list[Finding],
    suppressions: dict[Path, dict[int, set[str]]],
) -> tuple[list[Finding], int]:
    """Drop findings a same-or-previous-line marker allows. Returns (kept, dropped).

    ``suppressions`` maps a file path to ``{line: {rule_id, ...}}``. A finding is
    dropped when its rule id is allowed on its own line or the line above (so a
    marker can sit on the declaration or just before it). Line ``0`` allows a
    rule anywhere in the file, for path/whole-file findings.
    """
    kept: list[Finding] = []
    dropped = 0
    for finding in findings:
        by_line = suppressions.get(finding.path, {})
        allowed = (
            by_line.get(finding.line, set())
            | by_line.get(finding.line - 1, set())
            | by_line.get(0, set())
        )
        if finding.rule_id in allowed:
            dropped += 1
            continue
        kept.append(finding)
    return kept, dropped


# --- Formatting ------------------------------------------------------------

def _display_path(finding: Finding, project_root: Path) -> str:
    try:
        return finding.path.relative_to(project_root).as_posix()
    except ValueError:
        return finding.path.as_posix()


def sort_findings(findings: list[Finding]) -> list[Finding]:
    return sorted(
        findings,
        key=lambda f: (SEVERITY_ORDER[f.severity], f.path.as_posix(), f.line, f.rule_id),
    )


def format_text(findings: list[Finding], project_root: Path, guard_name: str, dropped: int) -> str:
    lines: list[str] = []
    counts = {sev: 0 for sev in Severity}
    for finding in findings:
        counts[finding.severity] += 1
    summary = ", ".join(f"{counts[sev]} {sev.value}" for sev in Severity if counts[sev])
    lines.append(f"{guard_name} failed: {len(findings)} issue(s) ({summary}).")
    if dropped:
        lines.append(f"({dropped} finding(s) suppressed by allow markers.)")
    for finding in sort_findings(findings):
        display = _display_path(finding, project_root)
        lines.append(
            f"{display}:{finding.line}:{finding.column}: "
            f"[{finding.severity.value.upper()} {finding.rule_id}] {finding.message}"
        )
        if finding.guidance:
            lines.append(f"    fix: {finding.guidance}")
    return "\n".join(lines)


def format_json(findings: list[Finding], project_root: Path) -> str:
    import json

    payload = [
        {
            "path": _display_path(finding, project_root),
            "line": finding.line,
            "column": finding.column,
            "ruleId": finding.rule_id,
            "severity": finding.severity.value,
            "message": finding.message,
            "guidance": finding.guidance,
            "skill": RULES[finding.rule_id].skill if finding.rule_id in RULES else "",
        }
        for finding in sort_findings(findings)
    ]
    return json.dumps({"findings": payload, "count": len(payload)}, indent=2)


def format_sarif(findings: list[Finding], project_root: Path, guard_name: str) -> str:
    import json

    rule_ids = sorted({finding.rule_id for finding in findings if finding.rule_id in RULES})
    sarif_rules = [
        {
            "id": rule_id,
            "name": rule_id,
            "shortDescription": {"text": RULES[rule_id].summary},
            "fullDescription": {"text": RULES[rule_id].guidance},
            "helpUri": "",
            "properties": {"skill": RULES[rule_id].skill, "severity": RULES[rule_id].severity.value},
            "defaultConfiguration": {"level": SARIF_LEVEL[RULES[rule_id].severity]},
        }
        for rule_id in rule_ids
    ]
    results = [
        {
            "ruleId": finding.rule_id,
            "level": SARIF_LEVEL[finding.severity],
            "message": {"text": finding.message},
            "locations": [
                {
                    "physicalLocation": {
                        "artifactLocation": {"uri": _display_path(finding, project_root)},
                        "region": {"startLine": finding.line, "startColumn": finding.column},
                    }
                }
            ],
        }
        for finding in sort_findings(findings)
    ]
    sarif = {
        "$schema": "https://json.schemastore.org/sarif-2.1.0.json",
        "version": "2.1.0",
        "runs": [
            {
                "tool": {"driver": {"name": guard_name, "rules": sarif_rules}},
                "results": results,
            }
        ],
    }
    return json.dumps(sarif, indent=2)


def explain(rule_id: str | None) -> str:
    if rule_id and rule_id in RULES:
        rule = RULES[rule_id]
        return (
            f"{rule.id}  [{rule.severity.value.upper()}]\n"
            f"  what : {rule.summary}\n"
            f"  fix  : {rule.guidance}\n"
            f"  skill: {rule.skill}\n"
            f"  roles: {', '.join(sorted(rule.roles)) or 'any'}"
        )
    lines = ["Rules (id  severity  summary):"]
    for rule in sorted(RULES.values(), key=lambda r: (SEVERITY_ORDER[r.severity], r.id)):
        lines.append(f"  {rule.id:<16} {rule.severity.value:<8} {rule.summary}")
    return "\n".join(lines)


# --- Paths -----------------------------------------------------------------

PROJECT_ROOT = Path(__file__).resolve().parents[2]
ECS_HPP = PROJECT_ROOT / "Source" / "Core" / "ecs.hpp"

IGNORED_PARTS = {
    ".git",
    "Binaries",
    "DerivedDataCache",
    "Intermediate",
    "Saved",
    "__pycache__",
}


def has_ignored_part(path: Path) -> bool:
    return any(part in IGNORED_PARTS for part in path.parts)


def rel(path: Path, root: Path = PROJECT_ROOT) -> str:
    try:
        return path.relative_to(root).as_posix()
    except ValueError:
        return path.as_posix()


def iter_files(roots: list[Path], suffixes: set[str] | None = None) -> list[Path]:
    """Every file under the roots, ignoring build dirs; optional suffix filter."""
    files: list[Path] = []
    for spec in roots:
        root = spec if spec.is_absolute() else PROJECT_ROOT / spec
        if root.is_file():
            files.append(root)
        elif root.is_dir():
            files.extend(child for child in root.rglob("*") if child.is_file())
    return sorted(
        path
        for path in files
        if not has_ignored_part(path) and (suffixes is None or path.suffix in suffixes)
    )


# --- Canonical tokenizer ---------------------------------------------------
# One tokenizer for both naming guards, so path/domain token comparisons stay
# consistent instead of drifting between two local copies.

def camel_tokens(value: str) -> list[str]:
    if not value:
        return []
    chunks = re.findall(r"[A-Z]+(?=[A-Z][a-z]|[0-9]|\b)|[A-Z]?[a-z]+|[0-9]+", value)
    return chunks or [value]


def normalize(value: str) -> str:
    """Lowercase, non-alphanumerics to underscores (no plural folding)."""
    return re.sub(r"[^a-z0-9]+", "_", value.lower()).strip("_")


def normalize_token(value: str) -> str:
    """Normalize one token and fold a trailing plural to its singular."""
    normalized = value.replace("-", "_").lower()
    if normalized.endswith("ies") and len(normalized) > 3:
        return normalized[:-3] + "y"
    if normalized.endswith("s") and not normalized.endswith("ss") and len(normalized) > 1:
        return normalized[:-1]
    return normalized


def normalized_tokens(tokens: list[str] | tuple[str, ...]) -> list[str]:
    return [normalize_token(token) for token in tokens]


def folder_tokens(part: str) -> list[str]:
    tokens: list[str] = []
    for piece in re.split(r"[_\-\s]+", part):
        tokens.extend(camel_tokens(piece))
    return tokens


def pluralize(atom: str) -> str:
    if atom.endswith("y"):
        return atom[:-1] + "ies"
    if atom.endswith("s"):
        return atom
    return atom + "s"


def snake(camel: str) -> str:
    return "_".join(token.lower() for token in camel_tokens(camel))


# --- ECS vocabulary derived from ecs.hpp -----------------------------------
# The naming doctrine's ECS section vocabulary is sourced from the ECS core, not
# a hand-maintained Python copy: add a domain kind or registry section in
# ecs.hpp and the naming guards track it.

@lru_cache(maxsize=1)
def _ecs_source() -> str:
    return ECS_HPP.read_text(encoding="utf-8", errors="replace") if ECS_HPP.exists() else ""


def _struct_array_fields(struct_name: str) -> set[str]:
    match = re.search(
        r"struct\s+" + re.escape(struct_name) + r"\s*\{(.*?)\};",
        _ecs_source(),
        re.DOTALL,
    )
    if not match:
        return set()
    fields = re.findall(r"TArray<[^;]+>\s+([A-Za-z][A-Za-z0-9_]*)\s*;", match.group(1))
    return {snake(field) for field in fields}


@lru_cache(maxsize=1)
def ecs_domain_kinds() -> frozenset[str]:
    """The EDomainKind values from ecs.hpp, lowercased (excluding Unknown)."""
    match = re.search(r"enum\s+class\s+EDomainKind\s*\{([^}]*)\}", _ecs_source())
    if not match:
        return frozenset()
    names = re.findall(r"[A-Za-z][A-Za-z0-9_]*", match.group(1))
    return frozenset(name.lower() for name in names if name != "Unknown")


@lru_cache(maxsize=1)
def ecs_section_keys() -> frozenset[str]:
    """The ECS registry section keys, derived from ecs.hpp.

    FDomainNode's TArray section fields (component_schemas, capabilities,
    systems, resources, events) plus the Component/Entity runtime stores and the
    domain registry container. If ecs.hpp cannot be read the set is empty and
    section-mixing rules simply do not fire, rather than enforcing a stale copy.
    """
    sections = {field for field in _struct_array_fields("FDomainNode") if field != "children"}
    sections |= {pluralize(kind) for kind in ecs_domain_kinds() if kind in {"component", "entity"}}
    if re.search(r"\bstruct\s+FDomainRegistry\b", _ecs_source()):
        sections |= {"domains", "domain_registry"}
    return frozenset(sections)


# --- JSON suppression ------------------------------------------------------

NAMING_ALLOW_KEY = "$naming-allow"


def json_naming_allow(value: object) -> set[str]:
    """Rule ids listed under a top-level `$naming-allow` key in a JSON object."""
    if not isinstance(value, dict):
        return set()
    allow = value.get(NAMING_ALLOW_KEY)
    if isinstance(allow, str):
        allow = [allow]
    if not isinstance(allow, list):
        return set()
    return {item for item in allow if isinstance(item, str) and re.fullmatch(r"[A-Z]+-[A-Z]+-\d+", item)}
