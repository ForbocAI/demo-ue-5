#pragma once
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

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

/** User Story: As a systems bots behavior consumer, I need to invoke bot ticked through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FBotTickPayload> &BotTicked() */
inline const rtk::ActionCreator<FBotTickPayload> &BotTicked() {
  static const func::Lazy<rtk::ActionCreator<FBotTickPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotTickPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotTickPayload>(TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV52609F6C3F6E));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots behavior consumer, I need to invoke bot moved through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FBotMovePayload> &BotMoved() */
inline const rtk::ActionCreator<FBotMovePayload> &BotMoved() {
  static const func::Lazy<rtk::ActionCreator<FBotMovePayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotMovePayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotMovePayload>(TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV775CF6FBC9BF));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots behavior consumer, I need to invoke bot damage taken through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FBotDamageTakenPayload> &BotDamageTaken() */
inline const rtk::ActionCreator<FBotDamageTakenPayload> &BotDamageTaken() {
  static const func::Lazy<rtk::ActionCreator<FBotDamageTakenPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotDamageTakenPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotDamageTakenPayload>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV0BA587392E1A));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots behavior consumer, I need to invoke bot enemy spotted through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FBotEnemySpottedPayload> &BotEnemySpotted() */
inline const rtk::ActionCreator<FBotEnemySpottedPayload> &BotEnemySpotted() {
  static const func::Lazy<rtk::ActionCreator<FBotEnemySpottedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotEnemySpottedPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotEnemySpottedPayload>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV8516462E517B));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots behavior consumer, I need to invoke bot attack requested through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FBotAttackRequestedPayload> & BotAttackRequested() */
inline const rtk::ActionCreator<FBotAttackRequestedPayload> &
BotAttackRequested() {
  static const func::Lazy<rtk::ActionCreator<FBotAttackRequestedPayload>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FBotAttackRequestedPayload> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FBotAttackRequestedPayload>(
                TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGVA95BCA2E03D0));
          });
  return func::eval(Creator);
}

/** User Story: As a systems bots behavior consumer, I need to invoke bot flee requested through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FBotFleeRequestedPayload> & BotFleeRequested() */
inline const rtk::ActionCreator<FBotFleeRequestedPayload> &
BotFleeRequested() {
  static const func::Lazy<rtk::ActionCreator<FBotFleeRequestedPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotFleeRequestedPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotFleeRequestedPayload>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV9FB2706658CC));
      });
  return func::eval(Creator);
}

} // namespace BotCoreActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
