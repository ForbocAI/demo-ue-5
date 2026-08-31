#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"
#include "Features/Components/Data/Settings/Registry/Automation/Scenarios/Bot/Orchestrator/Protocol/Assertions/AssertionsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Bot::FGroups,
                       StateCreation, Reducers, Movement, Combat, Awareness,
                       TickUpdate);

template <> struct TJsonSettingsRegistry<Automation::Bot::FCaseLabels> {
  /** User Story: As a bot automation settings consumer, I need to access registered case-label fields through a stable signature so scenarios remain data-driven. @fn static const TArray<TField<Automation::Bot::FCaseLabels>> &Fields() */
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

JSON_SETTINGS_REGISTRY(Automation::Bot::FStateAssertions,
                       Name, Health, Phase, IdValid);
JSON_SETTINGS_REGISTRY(Automation::Bot::FMovementAssertions, PositionX);
JSON_SETTINGS_REGISTRY(Automation::Bot::FCombatAssertions,
                       HealthReduced, PhaseCombat, PhaseFlee);
JSON_SETTINGS_REGISTRY(Automation::Bot::FAwarenessAssertions,
                       HasAggro, TimeSinceSeen, KnownPlayerPositionX,
                       PhaseCombat);
JSON_SETTINGS_REGISTRY(Automation::Bot::FTickAssertions,
                       TickCount, InitiallyAggro, LostAggro);

template <> struct TJsonSettingsRegistry<Automation::Bot::FAssertions> {
  /** User Story: As a bot automation settings consumer, I need typed assertion fields through a stable signature so labels are domain-owned and order-independent. @fn static const TArray<TField<Automation::Bot::FAssertions>> &Fields() */
  static const TArray<TField<Automation::Bot::FAssertions>> &Fields() {
    static const TArray<TField<Automation::Bot::FAssertions>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FAssertions,
                ReadSettingsWith<Automation::Bot::FStateAssertions>(
                    JSON_SETTINGS_ATOMS(Name, Health, Phase, IdValid)),
                State),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FAssertions,
                ReadSettingsWith<Automation::Bot::FMovementAssertions>(
                    JSON_SETTINGS_ATOMS(PositionX)),
                Movement),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FAssertions,
                ReadSettingsWith<Automation::Bot::FCombatAssertions>(
                    JSON_SETTINGS_ATOMS(HealthReduced, PhaseCombat,
                                        PhaseFlee)),
                Combat),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FAssertions,
                ReadSettingsWith<Automation::Bot::FAwarenessAssertions>(
                    JSON_SETTINGS_ATOMS(HasAggro, TimeSinceSeen,
                                        KnownPlayerPositionX, PhaseCombat)),
                Awareness),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FAssertions,
                ReadSettingsWith<Automation::Bot::FTickAssertions>(
                    JSON_SETTINGS_ATOMS(TickCount, InitiallyAggro,
                                        LostAggro)),
                Tick)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Bot::FOrchestratorGroups,
                       Registration, Cycle, RuntimeStore, Protocol);
JSON_SETTINGS_REGISTRY(Automation::Bot::FRegistrationCaseLabels,
                       RegisterBot, RegisterBots);
JSON_SETTINGS_REGISTRY(Automation::Bot::FCycleCaseLabels,
                       RespectObservationInterval, DispatchMovement);
JSON_SETTINGS_REGISTRY(Automation::Bot::FProtocolCaseLabels,
                       RejectRouteCrossover, ProjectAction,
                       ReduceProtocolLifecycle, ExecuteLiveFlows);
JSON_SETTINGS_REGISTRY(Automation::Bot::FProtocolRunSettings,
                       TimeoutSeconds, QualifiedDiagnosticMarker,
                       FailurePrefix);
JSON_SETTINGS_REGISTRY(Automation::Bot::FRootStateAssertions,
                       ThreeBotsInRootState, BotSelectable, HorseSelectable,
                       PositionSelectable, WorldPositionUpdated);
template <>
struct TJsonSettingsRegistry<Automation::Bot::FOrchestratorCaseLabels> {
  /** User Story: As a automation scenarios bot consumer, I need to invoke fields through a stable signature so the automation scenarios bot workflow remains explicit and composable. @fn static const TArray<TField<Automation::Bot::FOrchestratorCaseLabels>> &Fields() */
  static const TArray<TField<Automation::Bot::FOrchestratorCaseLabels>>
      &Fields() {
    static const TArray<TField<Automation::Bot::FOrchestratorCaseLabels>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorCaseLabels,
                ReadSettingsWith<Automation::Bot::FRegistrationCaseLabels>(
                    JSON_SETTINGS_ATOMS(RegisterBot, RegisterBots)),
                Registration),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorCaseLabels,
                ReadSettingsWith<Automation::Bot::FCycleCaseLabels>(
                    JSON_SETTINGS_ATOMS(RespectObservationInterval,
                                        DispatchMovement)),
                Cycle),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorCaseLabels,
                ReadSettingsWith<Automation::Bot::FProtocolCaseLabels>(
                    JSON_SETTINGS_ATOMS(RejectRouteCrossover, ProjectAction,
                                        ReduceProtocolLifecycle,
                                        ExecuteLiveFlows)),
                Protocol)};
    return RegisteredFields;
  }
};

template <>
struct TJsonSettingsRegistry<Automation::Bot::FOrchestratorAssertions> {
  /** User Story: As a automation scenarios bot consumer, I need to invoke fields through a stable signature so the automation scenarios bot workflow remains explicit and composable. @fn static const TArray<TField<Automation::Bot::FOrchestratorAssertions>> &Fields() */
  static const TArray<TField<Automation::Bot::FOrchestratorAssertions>>
      &Fields() {
    static const TArray<TField<Automation::Bot::FOrchestratorAssertions>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorAssertions,
                ReadSettingsWith<Automation::Bot::FRootStateAssertions>(
                    JSON_SETTINGS_ATOMS(
                        ThreeBotsInRootState, BotSelectable, HorseSelectable,
                        PositionSelectable, WorldPositionUpdated)),
                RootState),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorAssertions,
                ReadSettingsWith<Automation::Bot::FProtocolAssertions>(
                    JSON_SETTINGS_ATOMS(Core, Npc, Ghost, Log)),
                Protocol)};
    return RegisteredFields;
  }
};

template <>
struct TJsonSettingsRegistry<Automation::Bot::FOrchestratorSettings> {
  /** User Story: As a bot automation settings consumer, I need to access orchestrator fields through a stable signature so multi-bot scenarios remain data-driven. @fn static const TArray<TField<Automation::Bot::FOrchestratorSettings>> &Fields() */
  static const TArray<TField<Automation::Bot::FOrchestratorSettings>>
      &Fields() {
    static const TArray<TField<Automation::Bot::FOrchestratorSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings, Spec, MultiBotSpec,
                ProtocolSpec, Persona, WorldContextIndex),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings,
                ReadSettingsWith<Automation::Bot::FOrchestratorGroups>(
                    JSON_SETTINGS_ATOMS(Registration, Cycle, RuntimeStore,
                                        Protocol)),
                Groups),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings,
                ReadSettingsWith<Automation::Bot::FOrchestratorCaseLabels>(
                    JSON_SETTINGS_ATOMS(Registration, Cycle, Protocol)),
                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings,
                ReadSettingsWith<Automation::Bot::FOrchestratorAssertions>(
                    JSON_SETTINGS_ATOMS(RootState, Protocol)),
                Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FOrchestratorSettings,
                ReadSettingsWith<Automation::Bot::FProtocolRunSettings>(
                    JSON_SETTINGS_ATOMS(TimeoutSeconds,
                                        QualifiedDiagnosticMarker,
                                        FailurePrefix)),
                ProtocolRun)};
    return RegisteredFields;
  }
};

template <>
struct TJsonSettingsRegistry<Automation::Bot::FSettings> {
  /** User Story: As a bot automation settings consumer, I need to access bot scenario fields through a stable signature so scenario composition remains data-driven. @fn static const TArray<TField<Automation::Bot::FSettings>> &Fields() */
  static const TArray<TField<Automation::Bot::FSettings>> &Fields() {
    static const TArray<TField<Automation::Bot::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Bot::FSettings,
                                Spec, Tests),
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
                ReadSettingsWith<Automation::Bot::FAssertions>(
                    JSON_SETTINGS_ATOMS(State, Movement, Combat, Awareness,
                                        Tick)),
                Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FSettings,
                ReadSettingsWith<Automation::Bot::FOrchestratorSettings>(
                    JSON_SETTINGS_ATOMS(Spec, MultiBotSpec, ProtocolSpec,
                                        Persona, WorldContextIndex, Groups,
                                        Cases, Assertions, ProtocolRun)),
                Orchestrator)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
