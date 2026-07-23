#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Bot/Stats/BotStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotSettings {
  FString InitialName;
  float InitialHealth;
  float InitialMaxHealth;
  float InitialMana;
  float InitialMaxMana;
  float InitialStamina;
  float InitialMaxStamina;
  float MinimumHealth;
  FVector InitialPosition;
  FRotator InitialRotation;
  FVector InitialKnownPlayerPosition;
  float InitialTimeSinceSeenPlayer;
  float EnemySpottedTimeSinceSeenPlayer;
  bool bInitialHasAggro;
  bool bDefaultHazardOverlapping;
  bool bDefaultVisibilityCanSeeEnemy;
  int32 InitialPhase;
  int32 InitialTickCount;
  float AggroTimeoutSeconds;
  float DamageFleeHealthRatio;
  float PhaseFleeHealthRatio;
  float MovementArrivalDistanceSquared;
  float AggroPositionToleranceSquared;
  float DefaultMovementInterpSpeed;
  int32 PatrolGoalPriority;
  FString PatrolGoalIdFormat;
  bool bPatrolGoalInitialCompleted;
  bool bActiveGoalComponentHasActiveGoal;
  FStatPresetSettings TownspersonStats;
  FStatPresetSettings HorseStats;
  float ObservationIntervalSeconds;
  float InitialObservationTimeSeconds;
  bool bOrchestratorCanEverTick;
  float PatrolTickIntervalSeconds;
  float InitialPatrolPauseRemainingSeconds;
  bool bRegisteredBotActive;
  bool bPositionPayloadHasLocalLocation;
  bool bPositionPayloadHasWorldLocation;
  FString StartLog;
  FString RegisteredLogFormat;
  FString ProcessFailedLogFormat;
  FString ExecuteLogFormat;
  FString NullActorLabel;
  FString MoveActionType;
  FString AttackActionType;
  FVector MoveActionOffset;
  FString StateObservationFormat;
  int32 DefaultBehaviorState;
};

/** User Story: As a bot runtime consumer, I need to compare values for equality through a stable signature so runtime settings remain explicit and composable. @fn inline bool operator==(const FBotSettings &Left, const FBotSettings &Right) */
inline bool operator==(const FBotSettings &Left,
                       const FBotSettings &Right) {
  return Left.InitialName == Right.InitialName &&
         FMath::IsNearlyEqual(Left.InitialHealth, Right.InitialHealth) &&
         FMath::IsNearlyEqual(Left.InitialMaxHealth,
                              Right.InitialMaxHealth) &&
         FMath::IsNearlyEqual(Left.InitialMana, Right.InitialMana) &&
         FMath::IsNearlyEqual(Left.InitialMaxMana, Right.InitialMaxMana) &&
         FMath::IsNearlyEqual(Left.InitialStamina, Right.InitialStamina) &&
         FMath::IsNearlyEqual(Left.InitialMaxStamina,
                              Right.InitialMaxStamina) &&
         FMath::IsNearlyEqual(Left.MinimumHealth, Right.MinimumHealth) &&
         Left.InitialPosition == Right.InitialPosition &&
         Left.InitialRotation == Right.InitialRotation &&
         Left.InitialKnownPlayerPosition ==
             Right.InitialKnownPlayerPosition &&
         FMath::IsNearlyEqual(Left.InitialTimeSinceSeenPlayer,
                              Right.InitialTimeSinceSeenPlayer) &&
         FMath::IsNearlyEqual(Left.EnemySpottedTimeSinceSeenPlayer,
                              Right.EnemySpottedTimeSinceSeenPlayer) &&
         Left.bInitialHasAggro == Right.bInitialHasAggro &&
         Left.bDefaultHazardOverlapping ==
             Right.bDefaultHazardOverlapping &&
         Left.bDefaultVisibilityCanSeeEnemy ==
             Right.bDefaultVisibilityCanSeeEnemy &&
         Left.InitialPhase == Right.InitialPhase &&
         Left.InitialTickCount == Right.InitialTickCount &&
         FMath::IsNearlyEqual(Left.AggroTimeoutSeconds,
                              Right.AggroTimeoutSeconds) &&
         FMath::IsNearlyEqual(Left.DamageFleeHealthRatio,
                              Right.DamageFleeHealthRatio) &&
         FMath::IsNearlyEqual(Left.PhaseFleeHealthRatio,
                              Right.PhaseFleeHealthRatio) &&
         FMath::IsNearlyEqual(Left.MovementArrivalDistanceSquared,
                              Right.MovementArrivalDistanceSquared) &&
         FMath::IsNearlyEqual(Left.AggroPositionToleranceSquared,
                              Right.AggroPositionToleranceSquared) &&
         FMath::IsNearlyEqual(Left.DefaultMovementInterpSpeed,
                              Right.DefaultMovementInterpSpeed) &&
         Left.PatrolGoalPriority == Right.PatrolGoalPriority &&
         Left.PatrolGoalIdFormat == Right.PatrolGoalIdFormat &&
         Left.bPatrolGoalInitialCompleted ==
             Right.bPatrolGoalInitialCompleted &&
         Left.bActiveGoalComponentHasActiveGoal ==
             Right.bActiveGoalComponentHasActiveGoal &&
         Left.TownspersonStats == Right.TownspersonStats &&
         Left.HorseStats == Right.HorseStats &&
         FMath::IsNearlyEqual(Left.ObservationIntervalSeconds,
                              Right.ObservationIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.InitialObservationTimeSeconds,
                              Right.InitialObservationTimeSeconds) &&
         Left.bOrchestratorCanEverTick ==
             Right.bOrchestratorCanEverTick &&
         FMath::IsNearlyEqual(Left.PatrolTickIntervalSeconds,
                              Right.PatrolTickIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.InitialPatrolPauseRemainingSeconds,
                              Right.InitialPatrolPauseRemainingSeconds) &&
         Left.bRegisteredBotActive == Right.bRegisteredBotActive &&
         Left.bPositionPayloadHasLocalLocation ==
             Right.bPositionPayloadHasLocalLocation &&
         Left.bPositionPayloadHasWorldLocation ==
             Right.bPositionPayloadHasWorldLocation &&
         Left.StartLog == Right.StartLog &&
         Left.RegisteredLogFormat == Right.RegisteredLogFormat &&
         Left.ProcessFailedLogFormat == Right.ProcessFailedLogFormat &&
         Left.ExecuteLogFormat == Right.ExecuteLogFormat &&
         Left.NullActorLabel == Right.NullActorLabel &&
         Left.MoveActionType == Right.MoveActionType &&
         Left.AttackActionType == Right.AttackActionType &&
         Left.MoveActionOffset == Right.MoveActionOffset &&
         Left.StateObservationFormat == Right.StateObservationFormat &&
         Left.DefaultBehaviorState == Right.DefaultBehaviorState;
}

/** User Story: As a bot runtime consumer, I need to compare values for inequality through a stable signature so runtime settings remain explicit and composable. @fn inline bool operator!=(const FBotSettings &Left, const FBotSettings &Right) */
inline bool operator!=(const FBotSettings &Left,
                       const FBotSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
