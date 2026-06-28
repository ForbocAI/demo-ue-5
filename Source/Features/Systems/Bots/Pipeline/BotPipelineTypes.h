#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Core/BotCoreActions.h"
#include "Features/Systems/Bots/Core/BotCoreRuntimeTypes.h"

class AActor;

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FBotPipelinePayload {
  FString Id;
};

struct FBotPipelineState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  bool bReady = false;
};

struct FBotPipelineOverlapSnapshot {
  bool bOverlapping = false;
  float DamagePerSecond = 0.0f;
  AActor *HazardSource = nullptr;
};

struct FBotPipelineVisibilitySnapshot {
  bool bCanSeeEnemy = false;
  FVector EnemyPosition = FVector::ZeroVector;
  float Distance = 0.0f;
};

struct FBotPipelineMovementSnapshot {
  FVector TargetPosition = FVector::ZeroVector;
  float DeltaTime = 0.0f;
  float InterpSpeed = 300.0f;
};

struct FBotPipelineWorldSnapshot {
  FBotPipelineOverlapSnapshot HazardOverlap;
  FBotPipelineVisibilitySnapshot Visibility;
  FBotPipelineMovementSnapshot Movement;
  float DeltaTime = 0.0f;
};

struct FBotPipelineInputRequest {
  float DeltaTime = 0.0f;
  FBotPipelineOverlapSnapshot Hazard;
  FBotPipelineVisibilitySnapshot Visibility;
  FBotPipelineMovementSnapshot Movement;
};

struct FBotPipelineDefaultInputRequest {
  float DeltaTime = 0.0f;
};

struct FBotPipelineInputResult {
  FBotPipelineWorldSnapshot WorldSnapshot;
};

struct FBotPipelineHazardActionRequest {
  FBotCoreRuntimeState State;
  FBotPipelineOverlapSnapshot Overlap;
  float DeltaTime = 0.0f;
};

struct FBotPipelineMovementActionRequest {
  FBotCoreRuntimeState State;
  FBotPipelineMovementSnapshot Movement;
};

struct FBotPipelineAwarenessActionRequest {
  FBotCoreRuntimeState State;
  FBotPipelineVisibilitySnapshot Visibility;
};

struct FBotPipelinePhaseActionRequest {
  FBotCoreRuntimeState State;
};

struct FBotPipelineActionListAppendRequest {
  TArray<rtk::AnyAction> Actions;
  func::Maybe<rtk::AnyAction> Action;
};

struct FBotPipelineActionListRequest {
  FBotCoreRuntimeState State;
  FBotPipelineWorldSnapshot World;
};

struct FBotPipelineLogicResult {
  TArray<rtk::AnyAction> Actions;
};

struct FBotPipelineOutputResult {
  FBotCoreRuntimeState NewState;
  int32 ActionsDispatched = 0;
};

struct FBotPipelineTickInput {
  FBotCoreRuntimeState State;
  FBotPipelineWorldSnapshot World;
};

inline bool operator==(const FBotPipelineState &Left,
                       const FBotPipelineState &Right) {
  return Left.bReady == Right.bReady &&
         Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value);
}

inline bool operator!=(const FBotPipelineState &Left,
                       const FBotPipelineState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
