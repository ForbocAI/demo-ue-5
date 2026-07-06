#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FTownspersonDefaultsSettings {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
};


struct FBotStatPresetSettings {
  float MoveSpeed;
  float AwarenessRange;
  float Resolve;
  bool bCanTalk;
};


struct FBotRuntimeSettings {
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
  FVector InitialLastKnownPlayerPosition;
  float InitialTimeSinceLastSeenPlayer;
  float EnemySpottedTimeSinceLastSeenPlayer;
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
  FBotStatPresetSettings TownspersonStats;
  FBotStatPresetSettings HorseStats;
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


struct FTownspersonPresentationSettings {
  float CharacterHeightFeet;
  float CharacterShoulderWidthFeet;
  float PatrolPauseSeconds;
  float PromptAboveHeadFeet;
  float NameAbovePromptFeet;
  float DialogueAboveNameHeightRatio;
  float InteractionRadiusLots;
  float WalkSpeedHeightRatio;
  float MannequinScale;
  FVector MannequinOffsetFeet;
  FRotator MannequinRotation;
  float PromptTextScale;
  float DialogueTextScale;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
};


struct FHorsePresentationSettings {
  FString DefaultName;
  float HorseLengthFeet;
  float BodyHeightFeet;
  float LegHeightFeet;
  float NeckHeightFeet;
  float HeadHeightFeet;
  float SaddleHeightFeet;
  float PatrolPauseSeconds;
  float WalkSpeedHorseLengthRatio;
  float RouteArrivalLegRatio;
  float ImportedHorseScale;
  float MountedRiderScale;
  float NameTextWorldSizeFeet;
  FString HorseMeshPath;
  FString HorseWalkAnimationPath;
  FString RiderMeshPath;
  FString RiderWalkAnimationPath;
};


inline bool operator==(const FBotStatPresetSettings &Left,
                       const FBotStatPresetSettings &Right) {
  return FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve) &&
         Left.bCanTalk == Right.bCanTalk;
}

inline bool operator!=(const FBotStatPresetSettings &Left,
                       const FBotStatPresetSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotRuntimeSettings &Left,
                       const FBotRuntimeSettings &Right) {
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
         Left.InitialLastKnownPlayerPosition ==
             Right.InitialLastKnownPlayerPosition &&
         FMath::IsNearlyEqual(Left.InitialTimeSinceLastSeenPlayer,
                              Right.InitialTimeSinceLastSeenPlayer) &&
         FMath::IsNearlyEqual(Left.EnemySpottedTimeSinceLastSeenPlayer,
                              Right.EnemySpottedTimeSinceLastSeenPlayer) &&
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

inline bool operator!=(const FBotRuntimeSettings &Left,
                       const FBotRuntimeSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
