#!/usr/bin/env python3
"""Adapters boundary rules.

Adapters translate authored JSON, UE objects, ECS data, and entity-adapter
state. They do not dispatch, listen, or read the store.
"""

from __future__ import annotations

import re
from pathlib import Path

from boundary_common import Issue, combined_role_text


OWNED_FACTORY_RULES = (
    (
        re.compile(r"\brtk::createEntityAdapter\s*<"),
        {"adapters"},
        "entity adapters belong in Adapters files",
    ),
)

ENTITY_ADAPTER_EXPORT = re.compile(r"\bEntityAdapter\s*<|\bgetSelectors\s*\(")
CREATE_ENTITY_ADAPTER = re.compile(r"\brtk::createEntityAdapter\s*<")


def check(path: Path, text: str) -> list[Issue]:
    role_text = combined_role_text(path, text)
    if ENTITY_ADAPTER_EXPORT.search(role_text) and not CREATE_ENTITY_ADAPTER.search(role_text):
        return [
            Issue(
                path,
                1,
                "Adapters files that expose entity adapters must build them with rtk::createEntityAdapter; non-entity adapters stay translation helpers.",
            )
        ]
    return []
