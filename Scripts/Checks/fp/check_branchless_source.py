#!/usr/bin/env python3
"""Reject non-FP control flow in authored Source.

Built on the check_fp engine: findings carry stable ids/severities and the
guidance for each control-flow keyword is DERIVED from the func:: helper
vocabulary in Core/ue_fp.hpp, so a loop always points at the current fold/
traverse helpers and a switch at the current match/dispatch helpers. Beyond the
keyword ban it also flags the two ways people dodge it -- deep ternary chains
(mechanical if-replacement) and goto/do -- and the bool + out-param shape that
should return Maybe/Either so missing data is visible in the type.
"""

from __future__ import annotations

import argparse
from pathlib import Path
import re
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

from check_fp import (
    Finding,
    Rule,
    Severity,
    code_only,
    explain,
    format_json,
    format_sarif,
    format_text,
    fp_helpers_for,
    iter_source_files,
    line_number,
    register,
    PROJECT_ROOT,
    SOURCE_ROOT,
)


CONTROL_RE = re.compile(r"\b(if|else|for|while|switch|case)\b")
TERNARY_RE = re.compile(r"\?[^?:;{}]+:[^?:;{}]+\?[^?:;{}]+:")  # two or more chained ?:
JUMP_RE = re.compile(r"\b(goto|do)\b")
# bool Foo(..., Out& out) -- a boolean success flag plus a mutable out-param is
# the shape that should return Maybe/Either instead.
OUT_PARAM_RE = re.compile(
    r"\bbool\s+[A-Za-z_][A-Za-z0-9_]*\s*\([^;{)]*[A-Za-z0-9_>]\s*[&*]\s*[A-Za-z_][A-Za-z0-9_]*\s*\)\s*\{"
)


def _helper_hint(kind: str) -> str:
    helpers = fp_helpers_for(kind)
    return f"use func::{{{', '.join(helpers)}}}" if helpers else "use the func:: composition helpers"


CONTROL = register(
    Rule(
        id="FP-BRANCH-001",
        severity=Severity.HIGH,
        summary="non-FP control-flow keyword in authored Source",
        guidance=f"Loops: {_helper_hint('loop')}. Routing/branches: {_helper_hint('route')} plus func::match/ternary for small value selection. Explicit guards belong only at UE/effect boundaries.",
        skill="ue_fp.hpp: branching policy -- match/multi_match/Dispatcher + fold/filter/find/traverse instead of if/for/switch",
    )
)
TERNARY_ABUSE = register(
    Rule(
        id="FP-BRANCH-002",
        severity=Severity.MEDIUM,
        summary="chained ternary used as a mechanical if-replacement",
        guidance="ue_fp.hpp forbids mechanically replacing if with ternary chains. Use func::match for Maybe/Either, func::multi_match/Dispatcher case tables for routing.",
        skill="ue_fp.hpp: do not replace every if with ternary chains",
    )
)
JUMP = register(
    Rule(
        id="FP-BRANCH-003",
        severity=Severity.HIGH,
        summary="goto/do control flow",
        guidance="Replace with SDK functional-core recursion, folds, AsyncChain composition, or an explicit state-machine data table interpreted by one reusable step function.",
        skill="ue_fp.hpp: functional-core recursion/folds over open-ended loops",
    )
)
OUT_PARAM = register(
    Rule(
        id="FP-BRANCH-004",
        severity=Severity.MEDIUM,
        summary="bool return + mutable out-param instead of Maybe/Either",
        guidance="Return func::Maybe<T> or func::Either<E, T> so missing data is visible in the type instead of a success flag plus an out-param.",
        skill="ue_fp.hpp: prefer Maybe/Either-returning helpers so missing data is visible",
    )
)


def scan_file(path: Path) -> list[Finding]:
    code = code_only(path.read_text(encoding="utf-8", errors="replace"))
    findings: list[Finding] = []
    for match in CONTROL_RE.finditer(code):
        findings.append(
            Finding(path, line_number(code, match.start()), CONTROL.id, CONTROL.severity,
                    f"non-FP `{match.group(1)}` statement")
        )
    for match in TERNARY_RE.finditer(code):
        findings.append(
            Finding(path, line_number(code, match.start()), TERNARY_ABUSE.id, TERNARY_ABUSE.severity,
                    TERNARY_ABUSE.summary)
        )
    for match in JUMP_RE.finditer(code):
        findings.append(
            Finding(path, line_number(code, match.start()), JUMP.id, JUMP.severity,
                    f"`{match.group(1)}` control flow")
        )
    for match in OUT_PARAM_RE.finditer(code):
        findings.append(
            Finding(path, line_number(code, match.start()), OUT_PARAM.id, OUT_PARAM.severity,
                    OUT_PARAM.summary)
        )
    return findings


def find_findings() -> list[Finding]:
    return [finding for path in iter_source_files(SOURCE_ROOT) for finding in scan_file(path)]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--format", choices=("text", "json", "sarif"), default="text")
    parser.add_argument("--explain", nargs="?", const="", metavar="RULE-ID")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.explain is not None:
        print(explain(args.explain or None))
        return 0
    findings = find_findings()
    guard = "Branchless Source guard"
    if args.format == "json":
        print(format_json(findings, PROJECT_ROOT))
    elif args.format == "sarif":
        print(format_sarif(findings, PROJECT_ROOT, guard))
    elif findings:
        print(format_text(findings, PROJECT_ROOT, guard))
    else:
        print(f"{guard} passed.")
    return 1 if findings else 0


if __name__ == "__main__":
    sys.exit(main())
