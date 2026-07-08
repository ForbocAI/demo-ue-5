#!/usr/bin/env python3
"""Actions boundary rules.

Actions expose event-style action creators/facades. They may dispatch through
the one store boundary, but should not derive state or watch future actions.
"""

from __future__ import annotations

import re
from pathlib import Path

from boundary_common import Issue, combined_role_text, line_number


OWNED_FACTORY_RULES = (
    (
        re.compile(r"\brtk::createAction\s*<"),
        {"actions"},
        "rtk::createAction belongs in Actions files",
    ),
)

ACTION_CREATOR_EXPORT = re.compile(r"\brtk::ActionCreator\s*<|\bTypePrefix\s*\(")
CREATE_ACTION = re.compile(r"\brtk::createAction\s*<")
SETTER_STYLE_ACTION = re.compile(r"\b(?:Set|Replace)[A-Z][A-Za-z0-9_]*\s*\(")


def check(path: Path, text: str) -> list[Issue]:
    issues: list[Issue] = []
    role_text = combined_role_text(path, text)
    if ACTION_CREATOR_EXPORT.search(role_text) and not CREATE_ACTION.search(role_text):
        issues.append(
            Issue(
                path,
                1,
                "Actions files that expose action creators must build them with rtk::createAction so actions stay event-style metadata.",
            )
        )

    match = SETTER_STYLE_ACTION.search(text)
    if match:
        issues.append(
            Issue(
                path,
                line_number(text, match.start()),
                "Soft RTK upgrade: prefer event-style action names over setter/replacer facades. Dispatch what happened and let Slice own the transition.",
                "warning",
            )
        )

    return issues
