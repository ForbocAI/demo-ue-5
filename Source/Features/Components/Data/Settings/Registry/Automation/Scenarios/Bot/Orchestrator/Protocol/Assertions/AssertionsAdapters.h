#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Data/Settings/Automation/Bot/Orchestrator/Protocol/Assertions/ProtocolAssertionsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Bot::FProtocolCoreAssertions,
                       RouteCrossoverRejected, ActionProjected,
                       CorrelationPreserved, ProtocolFailurePreserved,
                       TownspersonAvailable);
JSON_SETTINGS_REGISTRY(Automation::Bot::FTraceAssertions,
                       CorrelationPresent, FamilyMatched, CommandMatched);
JSON_SETTINGS_REGISTRY(Automation::Bot::FResultAssertions,
                       ActorPresent, OutputPresent, ErrorAbsent);
JSON_SETTINGS_REGISTRY(Automation::Bot::FGhostResultAssertions,
                       ActorPresent, SessionPresent, OutputPresent,
                       QualifiedOutput, ErrorAbsent);
JSON_SETTINGS_REGISTRY(Automation::Bot::FLogAssertions,
                       SummaryNpcCorrelation, SummaryGhostCorrelation,
                       SummaryGhostSession, NpcOutputRedacted,
                       GhostOutputRedacted);

template <> struct TJsonSettingsRegistry<Automation::Bot::FNpcAssertions> {
  /** User Story: As a orchestrator protocol assertions consumer, I need to invoke fields through a stable signature so the orchestrator protocol assertions workflow remains explicit and composable. @fn static const TArray<TField<Automation::Bot::FNpcAssertions>> &Fields() */
  static const TArray<TField<Automation::Bot::FNpcAssertions>> &Fields() {
    static const TArray<TField<Automation::Bot::FNpcAssertions>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Bot::FNpcAssertions, Succeeded),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FNpcAssertions,
                ReadSettingsWith<Automation::Bot::FTraceAssertions>(
                    JSON_SETTINGS_ATOMS(CorrelationPresent, FamilyMatched,
                                        CommandMatched)),
                Trace),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FNpcAssertions,
                ReadSettingsWith<Automation::Bot::FResultAssertions>(
                    JSON_SETTINGS_ATOMS(ActorPresent, OutputPresent,
                                        ErrorAbsent)),
                Result)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<Automation::Bot::FGhostAssertions> {
  /** User Story: As a orchestrator protocol assertions consumer, I need to invoke fields through a stable signature so the orchestrator protocol assertions workflow remains explicit and composable. @fn static const TArray<TField<Automation::Bot::FGhostAssertions>> &Fields() */
  static const TArray<TField<Automation::Bot::FGhostAssertions>> &Fields() {
    static const TArray<TField<Automation::Bot::FGhostAssertions>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Bot::FGhostAssertions, Succeeded),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FGhostAssertions,
                ReadSettingsWith<Automation::Bot::FTraceAssertions>(
                    JSON_SETTINGS_ATOMS(CorrelationPresent, FamilyMatched,
                                        CommandMatched)),
                Trace),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FGhostAssertions,
                ReadSettingsWith<Automation::Bot::FGhostResultAssertions>(
                    JSON_SETTINGS_ATOMS(ActorPresent, SessionPresent,
                                        OutputPresent, QualifiedOutput,
                                        ErrorAbsent)),
                Result)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<Automation::Bot::FProtocolAssertions> {
  /** User Story: As a orchestrator protocol assertions consumer, I need to invoke fields through a stable signature so the orchestrator protocol assertions workflow remains explicit and composable. @fn static const TArray<TField<Automation::Bot::FProtocolAssertions>> &Fields() */
  static const TArray<TField<Automation::Bot::FProtocolAssertions>> &Fields() {
    static const TArray<TField<Automation::Bot::FProtocolAssertions>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FProtocolAssertions,
                ReadSettingsWith<Automation::Bot::FProtocolCoreAssertions>(
                    JSON_SETTINGS_ATOMS(
                        RouteCrossoverRejected, ActionProjected,
                        CorrelationPreserved, ProtocolFailurePreserved,
                        TownspersonAvailable)),
                Core),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FProtocolAssertions,
                ReadSettingsWith<Automation::Bot::FNpcAssertions>(
                    JSON_SETTINGS_ATOMS(Succeeded, Trace, Result)),
                Npc),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FProtocolAssertions,
                ReadSettingsWith<Automation::Bot::FGhostAssertions>(
                    JSON_SETTINGS_ATOMS(Succeeded, Trace, Result)),
                Ghost),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FProtocolAssertions,
                ReadSettingsWith<Automation::Bot::FLogAssertions>(
                    JSON_SETTINGS_ATOMS(
                        SummaryNpcCorrelation, SummaryGhostCorrelation,
                        SummaryGhostSession, NpcOutputRedacted,
                        GhostOutputRedacted)),
                Log)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
