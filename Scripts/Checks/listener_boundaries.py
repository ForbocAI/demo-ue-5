#!/usr/bin/env python3
"""Listeners boundary rules.

Listeners own reactive workflows that respond to later actions or state
changes. They are the place for long-lived reaction logic, not reducers/views.
"""

from __future__ import annotations

import re
from pathlib import Path

from boundary_common import Issue, combined_role_text, line_number


OWNED_FACTORY_RULES = (
    (
        re.compile(r"\bcreateListenerMiddleware\s*<|\bCreateRuntimeListenerMiddleware\b"),
        {"listeners"},
        "listener middleware belongs in Listeners files",
    ),
)

LISTENER_EXPORT = re.compile(r"\bListenerMiddleware\b|\bstartListening\b|\bStartListening\b")
CREATE_LISTENER = re.compile(r"\bcreateListenerMiddleware\s*<|\bCreateRuntimeListenerMiddleware\b")
APPENDED_LISTENER = re.compile(r"\bcreateListenerMiddleware[\s\S]{0,300}\bconcat\s*\(")


def check(path: Path, text: str) -> list[Issue]:
    issues: list[Issue] = []
    role_text = combined_role_text(path, text)
    if LISTENER_EXPORT.search(role_text) and not CREATE_LISTENER.search(role_text):
        issues.append(
            Issue(
                path,
                1,
                "Listeners files that expose listener middleware must create it with createListenerMiddleware and keep reactive workflows out of reducers/views.",
            )
        )

    match = APPENDED_LISTENER.search(text)
    if match:
        issues.append(
            Issue(
                path,
                line_number(text, match.start()),
                "RTK violation, forward target: listener middleware is prepended before default serializability checks, not appended after them.",
            )
        )

    return issues
