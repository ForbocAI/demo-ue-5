#pragma once

#include "Core/rtk.hpp"

class AActor;

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotTickPayload {
  float DeltaTime;
};

struct FBotMovePayload {
  FVector TargetLocation;
  float Speed;
};

struct FBotDamageTakenPayload {
  float Amount;
  AActor *Source;
};

struct FBotEnemySpottedPayload {
  FVector EnemyLocation;
};

struct FBotAttackRequestedPayload {
  AActor *Target;
};

struct FBotFleeRequestedPayload {
  FVector AwayFrom;
};

namespace BotCoreActions {

inline const rtk::ActionCreator<FBotTickPayload> &BotTicked() {
  static const func::Lazy<rtk::ActionCreator<FBotTickPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotTickPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotTickPayload>(TEXT("bots/core/ticked"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotMovePayload> &BotMoved() {
  static const func::Lazy<rtk::ActionCreator<FBotMovePayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotMovePayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotMovePayload>(TEXT("bots/core/moved"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotDamageTakenPayload> &BotDamageTaken() {
  static const func::Lazy<rtk::ActionCreator<FBotDamageTakenPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotDamageTakenPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotDamageTakenPayload>(
            TEXT("bots/core/damageTaken"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotEnemySpottedPayload> &BotEnemySpotted() {
  static const func::Lazy<rtk::ActionCreator<FBotEnemySpottedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotEnemySpottedPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
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
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FBotAttackRequestedPayload>(
                TEXT("bots/core/attackRequested"));
          });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FBotFleeRequestedPayload> &
BotFleeRequested() {
  static const func::Lazy<rtk::ActionCreator<FBotFleeRequestedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotFleeRequestedPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotFleeRequestedPayload>(
            TEXT("bots/core/fleeRequested"));
      });
  return func::eval(Creator);
}

} // namespace BotCoreActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
