#pragma once

#include "Core/rtk.hpp"

#include "CoreMinimal.h"

namespace ForbocAI {
namespace State {

struct FBotTickPayload {
  float DeltaTime = 0.0f;
};

struct FBotMovePayload {
  FVector TargetLocation = FVector::ZeroVector;
  float Speed = 0.0f;
};

struct FBotDamageTakenPayload {
  float Amount = 0.0f;
  AActor *Source = nullptr;
};

struct FBotEnemySpottedPayload {
  FVector EnemyLocation = FVector::ZeroVector;
};

struct FBotAttackRequestedPayload {
  AActor *Target = nullptr;
};

struct FBotFleeRequestedPayload {
  FVector AwayFrom = FVector::ZeroVector;
};

namespace BotCoreActions {

inline const rtk::ActionCreator<FBotTickPayload> &BotTicked() {
  static const func::Lazy<rtk::ActionCreator<FBotTickPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotTickPayload> {
        return rtk::createAction<FBotTickPayload>(TEXT("bots/core/ticked"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotMovePayload> &BotMoved() {
  static const func::Lazy<rtk::ActionCreator<FBotMovePayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotMovePayload> {
        return rtk::createAction<FBotMovePayload>(TEXT("bots/core/moved"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotDamageTakenPayload> &BotDamageTaken() {
  static const func::Lazy<rtk::ActionCreator<FBotDamageTakenPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotDamageTakenPayload> {
        return rtk::createAction<FBotDamageTakenPayload>(
            TEXT("bots/core/damageTaken"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotEnemySpottedPayload> &BotEnemySpotted() {
  static const func::Lazy<rtk::ActionCreator<FBotEnemySpottedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotEnemySpottedPayload> {
        return rtk::createAction<FBotEnemySpottedPayload>(
            TEXT("bots/core/enemySpotted"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotAttackRequestedPayload> &
BotAttackRequested() {
  static const func::Lazy<rtk::ActionCreator<FBotAttackRequestedPayload>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FBotAttackRequestedPayload> {
            return rtk::createAction<FBotAttackRequestedPayload>(
                TEXT("bots/core/attackRequested"));
          });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotFleeRequestedPayload> &
BotFleeRequested() {
  static const func::Lazy<rtk::ActionCreator<FBotFleeRequestedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotFleeRequestedPayload> {
        return rtk::createAction<FBotFleeRequestedPayload>(
            TEXT("bots/core/fleeRequested"));
      });
  return func::eval(Creator);
}

} // namespace BotCoreActions
} // namespace State
} // namespace ForbocAI
