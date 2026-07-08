#!/usr/bin/env python3
"""Selectors boundary rules.

Selectors expose derived reads from state. They should stay pure, avoid
dispatch/effects, and keep views away from store internals.
"""

from __future__ import annotations

import re
from pathlib import Path

from boundary_common import Issue, combined_role_text, line_number


OWNED_FACTORY_RULES = (
    (
        re.compile(r"\brtk::createSelector\s*<"),
        {"selectors", "slice"},
        "memoized selectors belong in Selectors files, or in Slice only when they are slice-local selectors",
    ),
)

SELECTOR_EXPORT = re.compile(r"\bSelector\s*<|\bselectAll\s*\(|\bselectById\s*\(")
SELECTOR_FACTORY = re.compile(r"\brtk::createSelector\s*<|\bgetSelectors\s*\(")
WHOLE_STATE_RETURN = re.compile(r"\breturn\s+State\s*;")


def check(path: Path, text: str) -> list[Issue]:
    issues: list[Issue] = []
    role_text = combined_role_text(path, text)
    if SELECTOR_EXPORT.search(role_text) and not SELECTOR_FACTORY.search(role_text):
        issues.append(
            Issue(
                path,
                1,
                "Selectors files that expose memoized/adapter selectors should use createSelector or adapter getSelectors; simple pure selectors may remain direct reads.",
            )
        )

    match = WHOLE_STATE_RETURN.search(text)
    if match:
        issues.append(
            Issue(
                path,
                line_number(text, match.start()),
                "Soft RTK upgrade: selectors should expose narrow derived reads instead of returning the whole slice/state and widening subscriptions.",
                "warning",
            )
        )

    return issues
