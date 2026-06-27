#pragma once

#include "Core/rtk.hpp"
#include "CoreMinimal.h"
#include "Features/Systems/Bots/Core/Actions.h"
#include "Features/Systems/Bots/Core/BotState.h"

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

struct FBotPipelineInputResult {
  FBotPipelineWorldSnapshot WorldSnapshot;
};

struct FBotPipelineLogicResult {
  TArray<rtk::AnyAction> Actions;
};

struct FBotPipelineOutputResult {
  ForbocAI::State::FBotState NewState;
  int32 ActionsDispatched = 0;
};

struct FBotPipelineTickInput {
  ForbocAI::State::FBotState State;
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
