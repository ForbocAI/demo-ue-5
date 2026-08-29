#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FStatPresetSettings, MoveSpeed, AwarenessRange,
                       Resolve, bCanTalk);

#define FORBOCAI_BOT_SETTING_FIELD(Group, GroupType, Field)                  \
  NestedSettingField(JSON_SETTING_ATOM(Field),                              \
                     NestedFieldMembers(&FBotSettings::Group,               \
                                        &GroupType::Field))

template <> struct TJsonSettingsRegistry<FBotSettings> {
/** User Story: As a bot registry consumer, I need to invoke fields through a stable signature so bot settings remain explicit and composable. @fn static const TArray<TField<FBotSettings>> &Fields() */
  static const TArray<TField<FBotSettings>> &Fields() {
    static const TArray<TField<FBotSettings>> RegisteredFields = {
        FORBOCAI_BOT_SETTING_FIELD(Health, FBotHealthSettings, InitialHealth),
        FORBOCAI_BOT_SETTING_FIELD(Health, FBotHealthSettings,
                                   InitialMaxHealth),
        FORBOCAI_BOT_SETTING_FIELD(Health, FBotHealthSettings, MinimumHealth),
        FORBOCAI_BOT_SETTING_FIELD(Resources, FBotResourceSettings,
                                   InitialMana),
        FORBOCAI_BOT_SETTING_FIELD(Resources, FBotResourceSettings,
                                   InitialMaxMana),
        FORBOCAI_BOT_SETTING_FIELD(Resources, FBotResourceSettings,
                                   InitialStamina),
        FORBOCAI_BOT_SETTING_FIELD(Resources, FBotResourceSettings,
                                   InitialMaxStamina),
        FORBOCAI_BOT_SETTING_FIELD(Spawn, FBotSpawnSettings, InitialName),
        FORBOCAI_BOT_SETTING_FIELD(Spawn, FBotSpawnSettings, InitialPosition),
        FORBOCAI_BOT_SETTING_FIELD(Spawn, FBotSpawnSettings, InitialRotation),
        FORBOCAI_BOT_SETTING_FIELD(Spawn, FBotSpawnSettings, InitialPhase),
        FORBOCAI_BOT_SETTING_FIELD(Spawn, FBotSpawnSettings, InitialTickCount),
        FORBOCAI_BOT_SETTING_FIELD(Awareness, FBotAwarenessSettings,
                                   InitialKnownPlayerPosition),
        FORBOCAI_BOT_SETTING_FIELD(Awareness, FBotAwarenessSettings,
                                   InitialTimeSinceSeenPlayer),
        FORBOCAI_BOT_SETTING_FIELD(Awareness, FBotAwarenessSettings,
                                   EnemySpottedTimeSinceSeenPlayer),
        FORBOCAI_BOT_SETTING_FIELD(Awareness, FBotAwarenessSettings,
                                   bInitialHasAggro),
        FORBOCAI_BOT_SETTING_FIELD(WorldSense, FBotWorldSenseSettings,
                                   bDefaultHazardOverlapping),
        FORBOCAI_BOT_SETTING_FIELD(WorldSense, FBotWorldSenseSettings,
                                   bDefaultVisibilityCanSeeEnemy),
        FORBOCAI_BOT_SETTING_FIELD(Awareness, FBotAwarenessSettings,
                                   AggroTimeoutSeconds),
        FORBOCAI_BOT_SETTING_FIELD(Behavior, FBotBehaviorSettings,
                                   DamageFleeHealthRatio),
        FORBOCAI_BOT_SETTING_FIELD(Behavior, FBotBehaviorSettings,
                                   PhaseFleeHealthRatio),
        FORBOCAI_BOT_SETTING_FIELD(Behavior, FBotBehaviorSettings,
                                   DefaultBehaviorState),
        FORBOCAI_BOT_SETTING_FIELD(Movement, FBotMovementSettings,
                                   MovementArrivalDistanceSquared),
        FORBOCAI_BOT_SETTING_FIELD(Movement, FBotMovementSettings,
                                   AggroPositionToleranceSquared),
        FORBOCAI_BOT_SETTING_FIELD(Movement, FBotMovementSettings,
                                   DefaultMovementInterpSpeed),
        FORBOCAI_BOT_SETTING_FIELD(Goals, FBotGoalSettings, PatrolGoalPriority),
        FORBOCAI_BOT_SETTING_FIELD(Goals, FBotGoalSettings, PatrolGoalIdFormat),
        FORBOCAI_BOT_SETTING_FIELD(Goals, FBotGoalSettings,
                                   bPatrolGoalInitialCompleted),
        FORBOCAI_BOT_SETTING_FIELD(Goals, FBotGoalSettings,
                                   bActiveGoalComponentHasActiveGoal),
        FORBOCAI_BOT_SETTING_FIELD(Schedule, FBotScheduleSettings,
                                   ObservationIntervalSeconds),
        FORBOCAI_BOT_SETTING_FIELD(Schedule, FBotScheduleSettings,
                                   InitialObservationTimeSeconds),
        FORBOCAI_BOT_SETTING_FIELD(Schedule, FBotScheduleSettings,
                                   PatrolTickIntervalSeconds),
        FORBOCAI_BOT_SETTING_FIELD(Schedule, FBotScheduleSettings,
                                   InitialPatrolPauseRemainingSeconds),
        FORBOCAI_BOT_SETTING_FIELD(Lifecycle, FBotLifecycleSettings,
                                   bOrchestratorCanEverTick),
        FORBOCAI_BOT_SETTING_FIELD(Lifecycle, FBotLifecycleSettings,
                                   bRegisteredBotActive),
        FORBOCAI_BOT_SETTING_FIELD(Projection, FBotProjectionSettings,
                                   bPositionPayloadHasLocalLocation),
        FORBOCAI_BOT_SETTING_FIELD(Projection, FBotProjectionSettings,
                                   bPositionPayloadHasWorldLocation),
        FORBOCAI_BOT_SETTING_FIELD(Projection, FBotProjectionSettings,
                                   StateObservationFormat),
        FORBOCAI_BOT_SETTING_FIELD(Diagnostics, FBotDiagnosticSettings,
                                   StartLog),
        FORBOCAI_BOT_SETTING_FIELD(Diagnostics, FBotDiagnosticSettings,
                                   RegisteredLogFormat),
        FORBOCAI_BOT_SETTING_FIELD(Diagnostics, FBotDiagnosticSettings,
                                   ProcessFailedLogFormat),
        FORBOCAI_BOT_SETTING_FIELD(Diagnostics, FBotDiagnosticSettings,
                                   ExecuteLogFormat),
        FORBOCAI_BOT_SETTING_FIELD(Diagnostics, FBotDiagnosticSettings,
                                   NullActorLabel),
        FORBOCAI_BOT_SETTING_FIELD(Actions, FBotActionSettings, MoveActionType),
        FORBOCAI_BOT_SETTING_FIELD(Actions, FBotActionSettings,
                                   AttackActionType),
        FORBOCAI_BOT_SETTING_FIELD(Actions, FBotActionSettings,
                                   MoveActionOffset),
        JSON_OBJECT_SETTING_FIELDS(
            FBotSettings,
            ReadSettingsWith<FStatPresetSettings>(JSON_SETTINGS_ATOMS(
                MoveSpeed, AwarenessRange, Resolve, bCanTalk)),
            TownspersonStats, HorseStats)};
    return RegisteredFields;
  }
};

#undef FORBOCAI_BOT_SETTING_FIELD

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
