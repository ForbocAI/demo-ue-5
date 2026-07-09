#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Bot::FGroups,
                       StateCreation, Reducers, Movement, Combat, Awareness,
                       TickUpdate);
template <> struct TJsonSettingsRegistry<Automation::Bot::FCaseLabels> {
  static const TArray<TField<Automation::Bot::FCaseLabels>> &Fields() {
    static const TArray<TField<Automation::Bot::FCaseLabels>>
        RegisteredFields = {
            NestedSettingField(
                JSON_SETTING_ATOM(CreateInitialState),
                NestedFieldMembers(
                    &Automation::Bot::FCaseLabels::State,
                    &Automation::Bot::FStateCaseLabels::CreateInitialState)),
            NestedSettingField(
                JSON_SETTING_ATOM(UpdatePosition),
                NestedFieldMembers(
                    &Automation::Bot::FCaseLabels::Movement,
                    &Automation::Bot::FMovementCaseLabels::UpdatePosition)),
            NestedSettingField(
                JSON_SETTING_ATOM(ReduceHealth),
                NestedFieldMembers(
                    &Automation::Bot::FCaseLabels::Combat,
                    &Automation::Bot::FCombatCaseLabels::ReduceHealth)),
            NestedSettingField(
                JSON_SETTING_ATOM(TransitionToCombat),
                NestedFieldMembers(
                    &Automation::Bot::FCaseLabels::Combat,
                    &Automation::Bot::FCombatCaseLabels::TransitionToCombat)),
            NestedSettingField(
                JSON_SETTING_ATOM(TransitionToFlee),
                NestedFieldMembers(
                    &Automation::Bot::FCaseLabels::Combat,
                    &Automation::Bot::FCombatCaseLabels::TransitionToFlee)),
            NestedSettingField(
                JSON_SETTING_ATOM(UpdateMemory),
                NestedFieldMembers(
                    &Automation::Bot::FCaseLabels::Awareness,
                    &Automation::Bot::FAwarenessCaseLabels::UpdateMemory)),
            NestedSettingField(
                JSON_SETTING_ATOM(IncrementTick),
                NestedFieldMembers(
                    &Automation::Bot::FCaseLabels::Tick,
                    &Automation::Bot::FTickCaseLabels::IncrementTick)),
            NestedSettingField(
                JSON_SETTING_ATOM(DecayAggro),
                NestedFieldMembers(
                    &Automation::Bot::FCaseLabels::Tick,
                    &Automation::Bot::FTickCaseLabels::DecayAggro))};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Bot::FOrchestratorGroups,
                       Registration, Cycle, RuntimeStore);

JSON_SETTINGS_REGISTRY(Automation::Bot::FOrchestratorCaseLabels,
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
                ReadSettingsWith<Automation::Bot::FOrchestratorCaseLabels>(
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
                ReadSettingsWith<Automation::Bot::FCaseLabels>(
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

template <>
struct TJsonSettingsRegistry<Automation::Conversation::UI::FDialogue> {
  static const TArray<TField<Automation::Conversation::UI::FDialogue>>
      &Fields() {
    static const TArray<TField<
        Automation::Conversation::UI::FDialogue>>
        RegisteredFields = {
            NestedSettingField(
                JSON_SETTING_ATOM(SpeakerName),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Speaker,
                    &Automation::Conversation::UI::FSpeaker::SpeakerName)),
            NestedSettingField(
                JSON_SETTING_ATOM(SpeakerRole),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Speaker,
                    &Automation::Conversation::UI::FSpeaker::SpeakerRole)),
            NestedSettingField(
                JSON_SETTING_ATOM(PlayerLine),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Exchange,
                    &Automation::Conversation::UI::FExchange::PlayerLine)),
            NestedSettingField(
                JSON_SETTING_ATOM(NpcReply),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Exchange,
                    &Automation::Conversation::UI::FExchange::NpcReply)),
            NestedSettingField(
                JSON_SETTING_ATOM(ExpectedTitle),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Expected,
                    &Automation::Conversation::UI::FExpected::ExpectedTitle)),
            NestedSettingField(
                JSON_SETTING_ATOM(ExpectedPlayerLine),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Expected,
                    &Automation::Conversation::UI::FExpected::
                        ExpectedPlayerLine)),
            NestedSettingField(
                JSON_SETTING_ATOM(ExpectedNpcReply),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Expected,
                    &Automation::Conversation::UI::FExpected::
                        ExpectedNpcReply))};
    return RegisteredFields;
  }
};

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
