#!/usr/bin/env python3
"""Thunks boundary rules.

Thunks own one imperative async workflow with dispatch/getState. Long-lived
reaction logic belongs in Listeners, and server cache usually belongs in RTK
Query.
"""

from __future__ import annotations

import re
from pathlib import Path

from boundary_common import Issue, combined_role_text, line_number


OWNED_FACTORY_RULES = (
    (
        re.compile(r"\brtk::createAsyncThunk\s*<"),
        {"thunks"},
        "rtk::createAsyncThunk belongs in Thunks files",
    ),
)

THUNK_EXPORT = re.compile(r"\brtk::ThunkAction\s*<|\bAsyncThunk\b|\bApi\.dispatch\b|\bApi\.getState\b")
THUNK_FACTORY = re.compile(r"\brtk::createAsyncThunk\s*<|\brtk::ThunkAction\s*<")
POLLING_GET_STATE = re.compile(r"\b(?:while|for)\s*\([^)]*\)[\s\S]{0,800}\b(?:Api\.getState|\.getState)\b")
SERVER_CACHE_WORKFLOW = re.compile(r"\b(?:FHttpModule|IHttpRequest|IHttpResponse|fetch\s*\(|Fetch[A-Z]|Http[A-Z])")


def check(path: Path, text: str) -> list[Issue]:
    issues: list[Issue] = []
    role_text = combined_role_text(path, text)

    if THUNK_EXPORT.search(role_text) and not THUNK_FACTORY.search(role_text):
        issues.append(
            Issue(
                path,
                1,
                "Thunks files that expose async workflows must use rtk::createAsyncThunk or typed ThunkAction and keep imperative effects out of Slice/View code.",
            )
        )

    for pattern, message in (
        (
            POLLING_GET_STATE,
            "Soft RTK upgrade: polling future state in a thunk fights the architecture. Move long-lived reactive workflows to Listeners.",
        ),
        (
            SERVER_CACHE_WORKFLOW,
            "Soft RTK upgrade: if this is request/response server cache, prefer an RTK Query createApi/injectEndpoints boundary over a hand-written thunk cache.",
        ),
    ):
        match = pattern.search(text)
        if match:
            issues.append(Issue(path, line_number(text, match.start()), message, "warning"))

    return issues
