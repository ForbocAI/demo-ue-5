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
  FVector MountedRiderOffsetFeet;
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

inline bool operator==(const FTownspersonDefaultsSettings &Left,
                       const FTownspersonDefaultsSettings &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Role == Right.Role && Left.Persona == Right.Persona &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine;
}

inline bool operator!=(const FTownspersonDefaultsSettings &Left,
                       const FTownspersonDefaultsSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonPresentationSettings &Left,
                       const FTownspersonPresentationSettings &Right) {
  return FMath::IsNearlyEqual(Left.CharacterHeightFeet,
                              Right.CharacterHeightFeet) &&
         FMath::IsNearlyEqual(Left.CharacterShoulderWidthFeet,
                              Right.CharacterShoulderWidthFeet) &&
         FMath::IsNearlyEqual(Left.PatrolPauseSeconds,
                              Right.PatrolPauseSeconds) &&
         FMath::IsNearlyEqual(Left.PromptAboveHeadFeet,
                              Right.PromptAboveHeadFeet) &&
         FMath::IsNearlyEqual(Left.NameAbovePromptFeet,
                              Right.NameAbovePromptFeet) &&
         FMath::IsNearlyEqual(Left.DialogueAboveNameHeightRatio,
                              Right.DialogueAboveNameHeightRatio) &&
         FMath::IsNearlyEqual(Left.InteractionRadiusLots,
                              Right.InteractionRadiusLots) &&
         FMath::IsNearlyEqual(Left.WalkSpeedHeightRatio,
                              Right.WalkSpeedHeightRatio) &&
         FMath::IsNearlyEqual(Left.MannequinScale, Right.MannequinScale) &&
         Left.MannequinOffsetFeet.Equals(Right.MannequinOffsetFeet) &&
         Left.MannequinRotation.Equals(Right.MannequinRotation) &&
         FMath::IsNearlyEqual(Left.PromptTextScale,
                              Right.PromptTextScale) &&
         FMath::IsNearlyEqual(Left.DialogueTextScale,
                              Right.DialogueTextScale) &&
         Left.MeshPath == Right.MeshPath &&
         Left.AnimationBlueprintClassPath ==
             Right.AnimationBlueprintClassPath;
}

inline bool operator!=(const FTownspersonPresentationSettings &Left,
                       const FTownspersonPresentationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FHorsePresentationSettings &Left,
                       const FHorsePresentationSettings &Right) {
  return Left.DefaultName == Right.DefaultName &&
         FMath::IsNearlyEqual(Left.HorseLengthFeet,
                              Right.HorseLengthFeet) &&
         FMath::IsNearlyEqual(Left.BodyHeightFeet, Right.BodyHeightFeet) &&
         FMath::IsNearlyEqual(Left.LegHeightFeet, Right.LegHeightFeet) &&
         FMath::IsNearlyEqual(Left.NeckHeightFeet, Right.NeckHeightFeet) &&
         FMath::IsNearlyEqual(Left.HeadHeightFeet, Right.HeadHeightFeet) &&
         FMath::IsNearlyEqual(Left.SaddleHeightFeet,
                              Right.SaddleHeightFeet) &&
         FMath::IsNearlyEqual(Left.PatrolPauseSeconds,
                              Right.PatrolPauseSeconds) &&
         FMath::IsNearlyEqual(Left.WalkSpeedHorseLengthRatio,
                              Right.WalkSpeedHorseLengthRatio) &&
         FMath::IsNearlyEqual(Left.RouteArrivalLegRatio,
                              Right.RouteArrivalLegRatio) &&
         FMath::IsNearlyEqual(Left.ImportedHorseScale,
                              Right.ImportedHorseScale) &&
         FMath::IsNearlyEqual(Left.MountedRiderScale,
                              Right.MountedRiderScale) &&
         Left.MountedRiderOffsetFeet.Equals(Right.MountedRiderOffsetFeet) &&
         FMath::IsNearlyEqual(Left.NameTextWorldSizeFeet,
                              Right.NameTextWorldSizeFeet) &&
         Left.HorseMeshPath == Right.HorseMeshPath &&
         Left.HorseWalkAnimationPath == Right.HorseWalkAnimationPath &&
         Left.RiderMeshPath == Right.RiderMeshPath &&
         Left.RiderWalkAnimationPath == Right.RiderWalkAnimationPath;
}

inline bool operator!=(const FHorsePresentationSettings &Left,
                       const FHorsePresentationSettings &Right) {
  return !(Left == Right);
}

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

inline bool operator!=(const FBotSettings &Left,
                       const FBotSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
