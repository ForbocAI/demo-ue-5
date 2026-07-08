#!/usr/bin/env python3
"""Enforce ue_fp cookbook composition and naming.

This guard is derived from Core/ue_fp.hpp's unary-composition cookbook. It
catches naming and call-shape violations that hide non-compositional bags:

* path/domain words repeated inside C++ identifiers;
* wrapper nouns such as Fixture/Bag/Manager/Helper/Utils;
* repeated typed factory families such as BindFloat/ComponentField/ReadMember;
* per-field registry entries instead of grouped declaration atoms;
* digit-suffixed helper/macro families such as READ_FIELD_1/FIELDS_32;
* wide scalar structs split into subdomain-owned data and
  recomposed by an adapter/fold.

The check derives context from paths and identifiers. It does not keep a domain
list, because domains stay free to move.
"""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
SOURCE_SUFFIXES = {".cpp", ".h", ".hpp"}
IGNORED_PARTS = {
    ".git",
    "Binaries",
    "DerivedDataCache",
    "Intermediate",
    "Saved",
    "__pycache__",
}

ROLE_SUFFIXES = {
    "Action",
    "Actions",
    "Adapter",
    "Adapters",
    "Listener",
    "Listeners",
    "Reducer",
    "Reducers",
    "Selector",
    "Selectors",
    "Setting",
    "Settings",
    "Slice",
    "Thunk",
    "Thunks",
    "Type",
    "Types",
    "View",
    "Views",
}

LAZY_NOUNS = {
    "Bag": "A Bag name hides unrelated data. Split by subdomain and compose at the adapter/fold boundary.",
    "Fixture": "A Fixture name turns test data into a bag. Move authored values to subdomain JSON and select the real values in the test.",
    "Helper": "A Helper name hides the actual role. Name the role boundary or move behavior into a reusable unary composer.",
    "Manager": "A Manager name hides ownership. Use RTK roles, ECS systems, adapters, or selectors with explicit data flow.",
    "Util": "A Util name hides the role. Move neutral behavior down or name the exact composition boundary.",
    "Utils": "A Utils name hides the role. Move neutral behavior down or name the exact composition boundary.",
}

COOKBOOK_CALL_RULES: tuple[tuple[re.Pattern[str], str], ...] = (
    (
        re.compile(r"\b(?:SettingField|ComponentField)\s*\("),
        "Per-field wrapper calls are not compositional. Pass grouped field/path atoms to one reusable composer; the composer owns expansion, conversion, validation, and folds.",
    ),
    (
        re.compile(r"\bBind(?:Bool|Text|String|Float|Double|Int|Vector|Rotator|Color)\s*(?:<|\()"),
        "Repeated typed Bind* factories are the cookbook's wrong shape. Push typed reader selection into the lower composer and leave this layer with declaration data.",
    ),
    (
        re.compile(r"\bReadMember\s*\("),
        "Do not hand-compose per-member readers. Declare source paths/atoms as data and let one generic reader/projector composer expand them.",
    ),
    (
        re.compile(r"\b(?:Required|Sequence)\s*\("),
        "Do not nest Required/Sequence helper calls inside declarations unless the behavior is genuinely one-off. Use grouped declaration atoms and a reusable composer.",
    ),
    (
        re.compile(r"\bfield_names\s*\("),
        "field_names plus field_readers is noun/type plumbing. Replace it with one grouped declaration consumed by a reusable composer.",
    ),
    (
        re.compile(r"\bfield_readers\s*\("),
        "field_readers plus field_names is noun/type plumbing. Replace it with one grouped declaration consumed by a reusable composer.",
    ),
    (
        re.compile(r"\bRegisteredFields\s*=\s*\{\s*\{", re.DOTALL),
        "Hand-written per-field registry entries leak the repeated shape. A registry declares grouped atoms/paths/conversions and lets one composer expand them.",
    ),
    (
        re.compile(r"\bRegistered(?:Rules|Cases|ProjectionPasses|Groups|Domain)\s*\([^;]*\bRegistered(?:Fields|Rules|Cases|ProjectionPasses)\s*\(", re.DOTALL),
        "Do not nest RegisteredFields/RegisteredRules/RegisteredCases inside another declaration. The outer boundary owns expansion; call sites supply grouped atoms.",
    ),
    (
        re.compile(r"\b(?:READ_FIELD|GET_FIELD_READER|FIELDS)_?\d+\b"),
        "Digit-suffixed helper/macro families are arity plumbing. Use one named composition boundary plus grouped declaration data.",
    ),
    (
        re.compile(r"\bCompose[A-Z][A-Za-z0-9_]*\s*\("),
        "Repeated ComposeX-style call families are abstraction leaks. Move the repeated shape into a higher composer and feed it grouped declarations.",
    ),
    (
        re.compile(r"\bProjectPayload[A-Z][A-Za-z0-9_]*\s*\("),
        "Repeated ProjectPayloadY-style call families are abstraction leaks. Move entity/source/field selection into one projector composer fed by declarations.",
    ),
)

WRAPPER_FAMILY_RE = re.compile(
    r"\b(?:struct|class)\s+([A-Z][A-Za-z0-9_]*(?:Seeds|Declarations|Registry|Catalog|Fields|Rules|Cases))\b"
)
SCALAR_FIELD_RE = re.compile(
    r"^\s*(?:const\s+)?(?:"
    r"FString|FName|FText|float|double|int32|int64|uint32|uint64|bool|"
    r"FVector|FRotator|FColor|FLinearColor|TArray\s*<\s*FString\s*>"
    r")\s+[A-Za-z_][A-Za-z0-9_]*\s*(?:=.*)?;",
    re.MULTILINE,
)
STRUCT_RE = re.compile(r"\bstruct\s+([A-Za-z_][A-Za-z0-9_]*)\s*\{", re.MULTILINE)
FUNCTION_RE = re.compile(
    r"(?:^|[;\n{}])\s*(?:[\w:<>&*\s]+\s+)?([A-Za-z_][A-Za-z0-9_:~]*)\s*\(",
    re.MULTILINE,
)
IDENTIFIER_RE = re.compile(r"\b[AEFISTU][A-Z][A-Za-z0-9_]*\b")
CONTROL_NAMES = {"if", "for", "while", "switch", "return", "sizeof", "TEXT"}
WIDE_SCALAR_FIELD_COUNT = 6
TEXT_ATOM_REPEAT_COUNT = 8
VIOLATION_PREFIX = "FP/ECS violation, forward target: "


@dataclass(frozen=True)
class Rule:
    id: str
    summary: str
    guidance: str


LAZY_NOUN = Rule(
    "FP-COMP-001",
    "identifier uses a lazy wrapper noun",
    "Name the exact role/domain boundary. Split vague bags/helpers into subdomain data and reusable composers.",
)
REPEATED_PATH_TOKEN = Rule(
    "FP-COMP-002",
    "identifier repeats folder/domain tokens",
    "Move domain words into folders/namespaces and keep local names role-shaped.",
)
COOKBOOK_CALL = Rule(
    "FP-COMP-003",
    "call shape repeats field/member composition",
    "Pass grouped declaration atoms to one reusable composer; the composer owns conversion and folds.",
)
WRAPPER_FAMILY = Rule(
    "FP-COMP-004",
    "struct/class is a noun wrapper family",
    "Put atoms/selectors/projectors/validators in grouped declarations and fold them with one composer.",
)
TEXT_ATOM_REPETITION = Rule(
    "FP-COMP-005",
    "file repeats platform text atoms",
    "Move authored atoms to grouped declarations or JSON and let generic helpers own TEXT conversion.",
)
WIDE_SCALAR_STRUCT = Rule(
    "FP-COMP-006",
    "wide scalar/list struct hides subdomains",
    "Split wide scalar/list records by subdomain and compose them through a reader/adapter fold.",
)


@dataclass(frozen=True)
class Issue:
    path: Path
    line: int
    rule_id: str
    message: str


@dataclass(frozen=True)
class StructNameItem:
    path: Path
    offset: int
    name: str
    candidates: tuple[str, ...]


@dataclass(frozen=True)
class StructNameContext:
    expected_by_key: dict[tuple[Path, int, str], str]


def display(path: Path) -> str:
    try:
        return path.relative_to(PROJECT_ROOT).as_posix()
    except ValueError:
        return path.as_posix()


def line_number(text: str, index: int) -> int:
    return text.count("\n", 0, index) + 1


def has_ignored_part(path: Path) -> bool:
    return any(part in IGNORED_PARTS for part in path.parts)


def camel_tokens(value: str) -> list[str]:
    if not value:
        return []
    return re.findall(
        r"[A-Z]+(?=[A-Z][a-z]|[0-9]|\b)|[A-Z]?[a-z]+|[0-9]+", value
    ) or [value]


def folder_tokens(part: str) -> list[str]:
    tokens: list[str] = []
    for piece in re.split(r"[_\-\s]+", part):
        tokens.extend(camel_tokens(piece))
    return tokens


def normalize(token: str) -> str:
    value = token.replace("-", "_").lower()
    if value.endswith("ies") and len(value) > 3:
        return value[:-3] + "y"
    if value.endswith("s") and not value.endswith("ss") and len(value) > 1:
        return value[:-1]
    return value


def strip_unreal_prefix(tokens: list[str]) -> list[str]:
    if tokens and tokens[0] in {"A", "E", "F", "I", "S", "T", "U"}:
        return tokens[1:]
    if (
        tokens
        and len(tokens[0]) > 1
        and tokens[0][0] in {"A", "E", "F", "I", "S", "T", "U"}
        and tokens[0][1:].isupper()
    ):
        return [tokens[0][1:]] + tokens[1:]
    return tokens


def strip_role_suffixes(tokens: list[str]) -> list[str]:
    result = list(tokens)
    while result and result[-1] in ROLE_SUFFIXES:
        result.pop()
    return result


def split_unreal_name(name: str) -> tuple[str, list[str]]:
    simple = name.rsplit("::", 1)[-1].lstrip("~")
    if (
        len(simple) > 1
        and simple[0] in {"A", "E", "F", "I", "S", "T", "U"}
        and simple[1].isupper()
    ):
        return simple[0], camel_tokens(simple[1:])
    return "", camel_tokens(simple)


def split_role_suffix(tokens: list[str]) -> tuple[list[str], list[str]]:
    result = list(tokens)
    suffix: list[str] = []
    while result and result[-1] in ROLE_SUFFIXES:
        suffix.insert(0, result.pop())
    return result, suffix


def shortest_qualifier_candidates(name: str) -> tuple[str, ...]:
    unreal_prefix, tokens = split_unreal_name(name)
    qualifier_tokens, suffix_tokens = split_role_suffix(tokens)
    if not qualifier_tokens or not suffix_tokens:
        return (name,)
    candidates = [
        unreal_prefix + "".join(qualifier_tokens[index:] + suffix_tokens)
        for index in range(len(qualifier_tokens) - 1, -1, -1)
    ]
    return tuple(dict.fromkeys(candidates))


def struct_name_context(paths: list[Path]) -> StructNameContext:
    items: list[dict] = []
    existing: dict[str, list[tuple[Path, int, str]]] = {}
    for path in paths:
        text = path.read_text(encoding="utf-8", errors="replace")
        for match in STRUCT_RE.finditer(text):
            name = match.group(1)
            key = (path, match.start(), name)
            existing.setdefault(name, []).append(key)
            candidates = shortest_qualifier_candidates(name)
            if candidates:
                items.append(
                    {"key": key, "name": name, "candidates": candidates, "index": 0}
                )

    while True:
        changed = False
        candidate_counts: dict[str, int] = {}
        for item in items:
            candidate_counts[item["candidates"][item["index"]]] = (
                candidate_counts.get(item["candidates"][item["index"]], 0) + 1
            )

        for item in items:
            candidate = item["candidates"][item["index"]]
            occupied_by_other = any(
                key != item["key"] for key in existing.get(candidate, [])
            )
            collides = candidate_counts[candidate] > 1 or occupied_by_other
            if collides and item["index"] < len(item["candidates"]) - 1:
                item["index"] += 1
                changed = True
        if not changed:
            break

    return StructNameContext(
        expected_by_key={
            item["key"]: item["candidates"][item["index"]] for item in items
        }
    )


def semantic_identifier_tokens(name: str) -> list[str]:
    simple = name.rsplit("::", 1)[-1].lstrip("~")
    return strip_role_suffixes(strip_unreal_prefix(camel_tokens(simple)))


def path_tokens(path: Path) -> set[str]:
    try:
        rel = path.relative_to(PROJECT_ROOT / "Source")
    except ValueError:
        try:
            rel = path.relative_to(PROJECT_ROOT)
        except ValueError:
            rel = path
    tokens: list[str] = []
    for part in rel.parent.parts:
        tokens.extend(folder_tokens(part))
    return {normalize(token) for token in tokens}


def lazy_noun_issues(path: Path, text: str, name: str, offset: int) -> list[Issue]:
    tokens = strip_unreal_prefix(camel_tokens(name.rsplit("::", 1)[-1].lstrip("~")))
    issues: list[Issue] = []
    for token in tokens:
        if token in LAZY_NOUNS:
            issues.append(
                Issue(path, line_number(text, offset), LAZY_NOUN.id, LAZY_NOUNS[token])
            )
    return issues


def repeated_path_token_issue(path: Path, text: str, name: str, offset: int) -> Issue | None:
    semantic_tokens = semantic_identifier_tokens(name)
    if not semantic_tokens:
        return None
    repeated = [
        token
        for token in semantic_tokens
        if normalize(token) in path_tokens(path)
    ]
    if not repeated:
        return None
    repeated_text = ", ".join(repeated)
    return Issue(
        path,
        line_number(text, offset),
        REPEATED_PATH_TOKEN.id,
        f"Identifier repeats folder/domain token(s): {repeated_text}. Move those words into folders/namespaces and keep the local name role-shaped.",
    )


def matching_brace_index(text: str, open_index: int) -> int | None:
    depth = 1
    index = open_index + 1
    while index < len(text):
        char = text[index]
        if char == "{":
            depth += 1
        elif char == "}":
            depth -= 1
            if depth == 0:
                return index
        index += 1
    return None


def struct_body(text: str, match: re.Match[str]) -> str | None:
    close = matching_brace_index(text, match.end() - 1)
    if close is None:
        return None
    return text[match.end() : close]


def wide_struct_issue(
    path: Path,
    text: str,
    match: re.Match[str],
    context: StructNameContext,
) -> Issue | None:
    name = match.group(1)
    body = struct_body(text, match)
    if body is None:
        return None
    scalar_fields = SCALAR_FIELD_RE.findall(body)
    if len(scalar_fields) <= WIDE_SCALAR_FIELD_COUNT:
        return None
    expected = context.expected_by_key.get((path, match.start(), name), name)
    qualifier = (
        f" Current name should shorten to `{expected}` if it remains in this domain; "
        "split records must use the same nearest-domain collision rule."
        if expected != name
        else " Split records must keep the shortest collision-free domain qualifier."
    )
    return Issue(
        path,
        line_number(text, match.start()),
        WIDE_SCALAR_STRUCT.id,
        f"Wide scalar struct `{name}` has {len(scalar_fields)} direct scalar/list fields. Split by subdomain and compose smaller records through a reader/adapter fold.{qualifier}",
    )


def cookbook_call_issues(path: Path, text: str) -> list[Issue]:
    issues: list[Issue] = []
    for pattern, message in COOKBOOK_CALL_RULES:
        for match in pattern.finditer(text):
            issues.append(Issue(path, line_number(text, match.start()), COOKBOOK_CALL.id, message))
    return issues


def wrapper_family_issues(path: Path, text: str) -> list[Issue]:
    issues: list[Issue] = []
    for match in WRAPPER_FAMILY_RE.finditer(text):
        name = match.group(1)
        issues.append(
            Issue(
                path,
                line_number(text, match.start()),
                WRAPPER_FAMILY.id,
                f"`{name}` looks like a noun wrapper family. If only nouns change, put atoms/selectors/projectors/validators in grouped declarations and fold them with one composer.",
            )
        )
    return issues


def text_atom_repetition_issue(path: Path, text: str) -> Issue | None:
    count = text.count("TEXT(\"")
    if count < TEXT_ATOM_REPEAT_COUNT:
        return None
    if "JSON_SETTINGS_ATOMS" in text or "JSON_REQUIRED_ATOMS" in text:
        return None
    return Issue(
        path,
        1,
        TEXT_ATOM_REPETITION.id,
        f"File repeats TEXT(...) atoms {count} times. Generic declaration helpers own platform string conversion; move authored atoms to grouped declarations or JSON.",
    )


def scan_file(path: Path, context: StructNameContext | None = None) -> list[Issue]:
    text = path.read_text(encoding="utf-8", errors="replace")
    name_context = context if context is not None else struct_name_context([path])
    issues: list[Issue] = []
    seen: set[tuple[int, str, str]] = set()

    def add(issue: Issue) -> None:
        key = (issue.line, issue.rule_id, issue.message)
        if key not in seen:
            seen.add(key)
            issues.append(issue)

    for match in STRUCT_RE.finditer(text):
        name = match.group(1)
        for issue in lazy_noun_issues(path, text, name, match.start()):
            add(issue)
        repeated = repeated_path_token_issue(path, text, name, match.start())
        if repeated is not None:
            add(repeated)
        wide = wide_struct_issue(path, text, match, name_context)
        if wide is not None:
            add(wide)

    for issue in cookbook_call_issues(path, text):
        add(issue)

    for issue in wrapper_family_issues(path, text):
        add(issue)

    text_atom_issue = text_atom_repetition_issue(path, text)
    if text_atom_issue is not None:
        add(text_atom_issue)

    for match in FUNCTION_RE.finditer(text):
        name = match.group(1)
        simple = name.rsplit("::", 1)[-1].lstrip("~")
        if simple in CONTROL_NAMES:
            continue
        for issue in lazy_noun_issues(path, text, simple, match.start()):
            add(issue)
        repeated = repeated_path_token_issue(path, text, simple, match.start())
        if repeated is not None:
            add(repeated)

    for match in IDENTIFIER_RE.finditer(text):
        name = match.group(0)
        for issue in lazy_noun_issues(path, text, name, match.start()):
            add(issue)
        repeated = repeated_path_token_issue(path, text, name, match.start())
        if repeated is not None:
            add(repeated)

    return issues


def iter_files(roots: list[Path]) -> list[Path]:
    files: list[Path] = []
    for root in roots:
        path = root if root.is_absolute() else PROJECT_ROOT / root
        if path.is_file() and path.suffix in SOURCE_SUFFIXES:
            files.append(path)
        elif path.is_dir():
            files.extend(
                child
                for child in path.rglob("*")
                if child.is_file() and child.suffix in SOURCE_SUFFIXES
            )
    return sorted(path for path in files if not has_ignored_part(path))


def find_issues(roots: list[Path]) -> list[Issue]:
    issues: list[Issue] = []
    files = iter_files(roots)
    context = struct_name_context(files)
    for path in files:
        issues.extend(scan_file(path, context))
    return sorted(issues, key=lambda issue: (display(issue.path), issue.line, issue.message))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("paths", nargs="*", type=Path, default=[Path("Source")])
    args = parser.parse_args()

    issues = find_issues(args.paths)
    if issues:
        print(f"Function/data composition naming guard failed: {len(issues)} issue(s).")
        for issue in issues:
            print(
                f"{display(issue.path)}:{issue.line}: "
                f"{issue.rule_id}: {VIOLATION_PREFIX}{issue.message}"
            )
        print("")
        print(
            "Forward architecture: folder/namespace boundaries own domain words; "
            "exact RTK/ECS role leaves own behavior; grouped declaration atoms feed "
            "one reusable composer that expands readers/projectors/validators/reducers/folds."
        )
        return 1

    print("Function/data composition naming guard passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
