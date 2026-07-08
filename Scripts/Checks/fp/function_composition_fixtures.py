#!/usr/bin/env python3
"""Meta-test for the FP/ECS composition guard.

Each fixture asserts a rule fires on a bad snippet and stays silent on a clean
snippet. This tests the linter, not Source; run it directly:

    python3 Scripts/Checks/function_composition_fixtures.py
"""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
import sys
import tempfile

_HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(_HERE))

import check_function_composition as comp


@dataclass(frozen=True)
class Case:
    name: str
    rel: str
    body: str
    expect: set[str]
    forbid: set[str] = frozenset()
    message_contains: tuple[str, ...] = ()


CASES: list[Case] = [
    Case(
        "wide-automation-settings",
        "Source/Features/Components/Data/Settings/Automation/Conversation/UI/ConversationUITypes.h",
        """
struct FAutomationSettings {
  int32 Count;
};

struct FConversationUITestAutomationSettings {
  TArray<FString> Labels;
  FString PlayerRole;
  FString PlayerText;
  FString ExpectedPlayerMessageText;
  float ExpectedPlayerMessageGreen;
  TArray<FString> History;
  int32 ExpectedHistoryCount;
  int32 TaggedHistoryIndex;
  FString ExpectedTaggedHistoryText;
};
""",
        {"FP-COMP-006"},
        message_contains=("`FTestAutomationSettings`",),
    ),
    Case(
        "wide-name-does-not-need-wrapper-or-path-repeat",
        "Source/Features/Demo/State/StateTypes.h",
        """
struct FRuntimeOptions {
  FString One;
  FString Two;
  FString Three;
  FString Four;
  FString Five;
  FString Six;
  FString Seven;
};
""",
        {"FP-COMP-006"},
    ),
    Case(
        "narrow-six-field-record",
        "Source/Features/Demo/State/StateTypes.h",
        """
struct FRuntimeOptions {
  FString One;
  FString Two;
  FString Three;
  FString Four;
  FString Five;
  FString Six;
};
""",
        set(),
        {"FP-COMP-006"},
    ),
    Case(
        "nested-domain-aggregate",
        "Source/Features/Components/Data/Settings/SettingsTypes.h",
        """
struct FAutomationSettings {
  FStoreAutomationSettings Store;
  FContentAssetsAutomationSettings ContentAssets;
  FRTKComplianceAutomationSettings RtkCompliance;
  FConversationUIAutomationSettings ConversationUI;
  FProtocolLoopAutomationSettings ProtocolLoop;
  FSpeechAutomationSettings Speech;
  FRenderingAutomationSettings Rendering;
};
""",
        set(),
        {"FP-COMP-006"},
    ),
    Case(
        "lazy-noun",
        "Source/Features/Demo/State/StateTypes.h",
        "struct FThingManager { int32 Count; };",
        {"FP-COMP-001"},
    ),
    Case(
        "template-registry-composer",
        "Source/Features/Demo/Data/Registry/RegistryAdapters.h",
        """
template <typename Value> struct TValueRegistry;

template <> struct TValueRegistry<FThing> {
  static int32 Apply() { return 1; }
};

struct FThingFields {
  FString Name;
};
""",
        {"FP-COMP-004"},
        message_contains=("`FThingFields`",),
    ),
]


def run_case(root: Path, case: Case) -> list[comp.Issue]:
    path = root / case.rel
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(case.body, encoding="utf-8")
    paths = [path]
    return comp.scan_file(
        path,
        comp.struct_name_context(paths),
        comp.qualified_name_context(paths),
    )


def main() -> int:
    failures: list[str] = []
    with tempfile.TemporaryDirectory() as tmp:
        root = Path(tmp)
        for case in CASES:
            issues = run_case(root, case)
            fired = {issue.rule_id for issue in issues}
            missing = case.expect - fired
            unexpected = case.forbid & fired
            if missing:
                failures.append(
                    f"{case.name}: expected {sorted(missing)} to fire, got {sorted(fired)}"
                )
            if unexpected:
                failures.append(
                    f"{case.name}: {sorted(unexpected)} fired but is forbidden"
                )
            for text in case.message_contains:
                if not any(text in issue.message for issue in issues):
                    failures.append(
                        f"{case.name}: expected a message containing {text!r}, got {[issue.message for issue in issues]}"
                    )

    if failures:
        print(f"Function composition fixtures FAILED: {len(failures)} case(s).")
        for failure in failures:
            print(f"  - {failure}")
        return 1
    print(f"Function composition fixtures passed: {len(CASES)} checks.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
