#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Bot::FGroups,
                       StateCreation, Reducers, Movement, Combat, Awareness,
                       TickUpdate);

JSON_SETTINGS_REGISTRY(Automation::Bot::FCases,
                       CreateInitialState, UpdatePosition, ReduceHealth,
                       TransitionToCombat, TransitionToFlee, UpdateMemory,
                       IncrementTick, DecayAggro);

JSON_SETTINGS_REGISTRY(Automation::Bot::FOrchestratorGroups,
                       Registration, Cycle, RuntimeStore);

JSON_SETTINGS_REGISTRY(Automation::Bot::FOrchestratorCases,
                       RegisterBot, RespectObservationInterval, RegisterBots,
                       DispatchMovement);

JSON_SETTINGS_REGISTRY(Automation::Bot::FOrchestratorAssertions,
                       ThreeBotsInRootState, BotSelectable, HorseSelectable,
                       PositionSelectable, WorldPositionUpdated);

template <>
struct TJsonSettingsRegistry<Automation::Bot::FOrchestratorSettings> {
  static const TArray<TField<Automation::Bot::FOrchestratorSettings>>
      &Fields() {
    static const TArray<
        TField<Automation::Bot::FOrchestratorSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings, Spec, MultiBotSpec,
                Persona, WorldContextIndex),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings,
                ReadSettingsWith<Automation::Bot::FOrchestratorGroups>(
                    JSON_SETTINGS_ATOMS(Registration, Cycle, RuntimeStore)),
                Groups),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings,
                ReadSettingsWith<Automation::Bot::FOrchestratorCases>(
                    JSON_SETTINGS_ATOMS(RegisterBot,
                                        RespectObservationInterval,
                                        RegisterBots, DispatchMovement)),
                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings,
                ReadSettingsWith<Automation::Bot::FOrchestratorAssertions>(
                    JSON_SETTINGS_ATOMS(
                        ThreeBotsInRootState, BotSelectable, HorseSelectable,
                        PositionSelectable, WorldPositionUpdated)),
                Assertions)};
    return RegisteredFields;
  }
};

template <>
struct TJsonSettingsRegistry<Automation::Bot::FSettings> {
  static const TArray<TField<Automation::Bot::FSettings>>
      &Fields() {
    static const TArray<TField<
        Automation::Bot::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Bot::FSettings,
                                Spec, Tests, Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FSettings,
                ReadSettingsWith<Automation::Bot::FGroups>(
                    JSON_SETTINGS_ATOMS(StateCreation, Reducers, Movement,
                                        Combat, Awareness, TickUpdate)),
                Groups),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FSettings,
                ReadSettingsWith<Automation::Bot::FCases>(
                    JSON_SETTINGS_ATOMS(CreateInitialState, UpdatePosition,
                                        ReduceHealth, TransitionToCombat,
                                        TransitionToFlee, UpdateMemory,
                                        IncrementTick, DecayAggro)),
                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FSettings,
                ReadSettingsWith<Automation::Bot::FOrchestratorSettings>(
                    JSON_SETTINGS_ATOMS(Spec, MultiBotSpec, Persona,
                                        WorldContextIndex, Groups, Cases,
                                        Assertions)),
                Orchestrator)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Pipeline::FTests, IdleTickAdvancesState,
                       HazardCausesDamage, AwarenessTriggersAggro,
                       FleeTransitionOnLowHealth, MultiBotIndependent,
                       DeterministicOrder);

JSON_SETTINGS_REGISTRY(Automation::Pipeline::FAssertions, TickCountAdvanced,
                       ActionDispatched, HealthUnchanged,
                       HealthReducedByHazard, HealthAfterHazard, BotHasAggro,
                       PhaseIsCombat, RemembersEnemyPosition,
                       PhaseTransitionedToFlee, BotsProcessed,
                       IdleBotFullHealth, HazardBotTookDamage,
                       AwareBotHasAggro, HealthDeterministic,
                       PositionDeterministic, PhaseDeterministic,
                       AggroDeterministic, ActionCountDeterministic);

template <> struct TJsonSettingsRegistry<Automation::Pipeline::FSettings> {
  static const TArray<TField<Automation::Pipeline::FSettings>> &Fields() {
    static const TArray<TField<Automation::Pipeline::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Pipeline::FSettings, Spec, Groups,
                                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Pipeline::FSettings,
                ReadSettingsWith<Automation::Pipeline::FTests>(
                    JSON_SETTINGS_ATOMS(
                        IdleTickAdvancesState, HazardCausesDamage,
                        AwarenessTriggersAggro, FleeTransitionOnLowHealth,
                        MultiBotIndependent, DeterministicOrder)),
                Tests),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Pipeline::FSettings,
                ReadSettingsWith<Automation::Pipeline::FAssertions>(
                    JSON_SETTINGS_ATOMS(
                        TickCountAdvanced, ActionDispatched, HealthUnchanged,
                        HealthReducedByHazard, HealthAfterHazard, BotHasAggro,
                        PhaseIsCombat, RemembersEnemyPosition,
                        PhaseTransitionedToFlee, BotsProcessed,
                        IdleBotFullHealth, HazardBotTookDamage,
                        AwareBotHasAggro, HealthDeterministic,
                        PositionDeterministic, PhaseDeterministic,
                        AggroDeterministic, ActionCountDeterministic)),
                Assertions)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FMessage, Role, Text,
                       ExpectedText, ExpectedGreen);

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FLine, Index,
                       ExpectedText);

template <>
struct TJsonSettingsRegistry<Automation::Conversation::UI::FHistory> {
  static const TArray<TField<Automation::Conversation::UI::FHistory>>
      &Fields() {
    static const TArray<TField<
        Automation::Conversation::UI::FHistory>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Conversation::UI::FHistory, Lines,
                                ExpectedCount),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FHistory,
                ReadSettingsWith<Automation::Conversation::UI::FLine>(
                    JSON_SETTINGS_ATOMS(Index, ExpectedText)),
                Tagged, Untagged)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FSubmitted, Input,
                       ExpectedText);

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FDialogue, SpeakerName,
                       SpeakerRole, PlayerLine, NpcReply, ExpectedTitle,
                       ExpectedPlayerLine, ExpectedNpcReply);

template <>
struct TJsonSettingsRegistry<Automation::Conversation::UI::FSettings> {
  static const TArray<TField<Automation::Conversation::UI::FSettings>>
      &Fields() {
    static const TArray<TField<
        Automation::Conversation::UI::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Conversation::UI::FSettings, Test,
                                Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FMessage>(
                    JSON_SETTINGS_ATOMS(Role, Text, ExpectedText,
                                        ExpectedGreen)),
                PlayerMessage),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FHistory>(
                    JSON_SETTINGS_ATOMS(Lines, ExpectedCount, Tagged,
                                        Untagged)),
                History),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FSubmitted>(
                    JSON_SETTINGS_ATOMS(Input, ExpectedText)),
                Submitted),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FDialogue>(
                    JSON_SETTINGS_ATOMS(
                        SpeakerName, SpeakerRole, PlayerLine, NpcReply,
                        ExpectedTitle, ExpectedPlayerLine, ExpectedNpcReply)),
                Dialogue)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
