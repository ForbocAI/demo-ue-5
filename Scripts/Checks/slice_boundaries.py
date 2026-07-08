#!/usr/bin/env python3
"""Slice boundary rules.

Slice owns createSlice assembly, initial state wiring, and pure reducer
transitions. Reducer helpers may live here, but side effects do not.
"""

from __future__ import annotations

import re
from pathlib import Path

from boundary_common import Issue, combined_role_text, line_number


OWNED_FACTORY_RULES = (
    (
        re.compile(r"\brtk::createSlice\s*<"),
        {"slice"},
        "rtk::createSlice belongs in Slice files",
    ),
)

SLICE_EXPORT = re.compile(r"\brtk::Slice\s*<|\bGetSlice\s*\(")
CREATE_SLICE = re.compile(r"\brtk::createSlice\s*<")
HAND_WRITTEN_REDUCER = re.compile(
    r"\brtk::createReducer\s*<"
    r"|\bswitch\s*\([^)]*(?:Action|action)\s*\.\s*(?:type|Type)[^)]*\)"
)
BLIND_PAYLOAD_REPLACE = re.compile(r"\breturn\s+(?:Action|action)\.PayloadValue\s*;")
VIEW_MODEL_STATE = re.compile(r"\b(?:ViewModel|ViewDefaults)\b")


def check(path: Path, text: str) -> list[Issue]:
    issues: list[Issue] = []
    role_text = combined_role_text(path, text)

    if SLICE_EXPORT.search(role_text) and not CREATE_SLICE.search(role_text):
        issues.append(
            Issue(
                path,
                1,
                "Slice files that expose GetSlice/rtk::Slice must assemble with rtk::createSlice in the same file or sibling Slice.cpp; reducer helpers may live in Slice without pretending to be store slices.",
            )
        )

    for pattern, message in (
        (
            HAND_WRITTEN_REDUCER,
            "Soft RTK upgrade: migrate hand-written/createReducer transitions into createSlice assembly unless this is a measured escape hatch.",
        ),
        (
            BLIND_PAYLOAD_REPLACE,
            "Soft RTK upgrade: reducers should own the slice shape instead of blindly replacing state from payloads unless the payload is an authoritative snapshot.",
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
