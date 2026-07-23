#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Behavior/BehaviorTypes.h"
#include "Features/Systems/Bots/Pipeline/BotsPipelineTypes.h"

class AActor;

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotPipelineOverlapSnapshot {
  bool bOverlapping;
  float DamagePerSecond;
  AActor *HazardSource;
};

struct FBotPipelineVisibilitySnapshot {
  bool bCanSeeEnemy;
  FVector EnemyPosition;
  float Distance;
};

struct FBotPipelineMovementSnapshot {
  FVector TargetPosition;
  float DeltaTime;
  float InterpSpeed;
};

struct FBotPipelineWorldSnapshot {
  FBotPipelineOverlapSnapshot HazardOverlap;
  FBotPipelineVisibilitySnapshot Visibility;
  FBotPipelineMovementSnapshot Movement;
  float DeltaTime;
};

struct FBotPipelineInputRequest {
  float DeltaTime;
  FBotPipelineOverlapSnapshot Hazard;
  FBotPipelineVisibilitySnapshot Visibility;
  FBotPipelineMovementSnapshot Movement;
};

struct FBotPipelineDefaultInputRequest {
  FBotCoreRuntimeState State;
  float DeltaTime;
};

struct FBotPipelineInputResult {
  FBotPipelineWorldSnapshot WorldSnapshot;
};

struct FBotPipelineHazardActionRequest {
  FBotCoreRuntimeState State;
  FBotPipelineOverlapSnapshot Overlap;
  float DeltaTime;
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
  int32 ActionsDispatched;
};

struct FBotPipelineTickInput {
  FBotCoreRuntimeState State;
  FBotPipelineWorldSnapshot World;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
