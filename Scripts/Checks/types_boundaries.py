#!/usr/bin/env python3
"""Types boundary rules.

Types stay inert: state, payload, entity, and dispatch declarations only. They
should not own side effects, store access, or non-serializable behavior.
"""

from __future__ import annotations

import re
from pathlib import Path

from boundary_common import Issue, line_number


OWNED_FACTORY_RULES: tuple[tuple[re.Pattern[str], set[str], str], ...] = ()

NON_SERIALIZABLE_STATE = re.compile(
    r"\b(?:TFunction|TSharedPtr|TUniquePtr|TWeakObjectPtr|TObjectPtr|std::function)\s*<"
    r"|\b(?:U[A-Za-z0-9_]+|A[A-Za-z0-9_]+|UWorld)\s*\*"
)
SCRATCHPAD_STATE = re.compile(r"\bLast[A-Z][A-Za-z0-9_]*\b")
VIEW_MODEL_STATE = re.compile(r"\b(?:ViewModel|ViewDefaults)\b")


def check(path: Path, text: str) -> list[Issue]:
    issues: list[Issue] = []
    for pattern, message in (
        (
            NON_SERIALIZABLE_STATE,
            "Soft RTK upgrade: persistent Types should stay serializable/inert; keep UE object pointers, shared ownership, and callbacks at Adapter/Thunk/Listener/View boundaries.",
        ),
        (
            SCRATCHPAD_STATE,
            "Soft RTK upgrade: Last*/scratchpad state often signals derived or transient data. Keep only authoritative state and derive read models with Selectors when possible.",
        ),
        (
            VIEW_MODEL_STATE,
            "Soft RTK upgrade: view models/defaults are usually derived reads. Prefer Selectors unless this is an explicit boundary payload.",
        ),
    ):
        match = pattern.search(text)
        if match:
            issues.append(Issue(path, line_number(text, match.start()), message, "warning"))
    return issues
