#include "Features/Systems/Bots/Position/Actions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionActions {

const rtk::ActionCreator<TArray<FBotPositionComponent>> &
BotPositionsSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotPositionComponent>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotPositionComponent>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FBotPositionComponent>>(
          TEXT("botPosition/seeded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotPositionComponent> &BotPositionUpserted() {
  static const func::Lazy<rtk::ActionCreator<FBotPositionComponent>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotPositionComponent> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotPositionComponent>(
            TEXT("botPosition/upserted"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved() {
  static const func::Lazy<rtk::ActionCreator<FBotPositionMoved>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotPositionMoved> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotPositionMoved>(TEXT("botPosition/moved"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotInitialPatrolLocationRequest> &
InitialPatrolObserved() {
  static const func::Lazy<rtk::ActionCreator<FBotInitialPatrolLocationRequest>>
      Creator =
          func::lazy(
              []() -> rtk::ActionCreator<FBotInitialPatrolLocationRequest> {
                // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
                return rtk::createAction<FBotInitialPatrolLocationRequest>(
                    TEXT("botPosition/initialPatrolObserved"));
              });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotPatrolAdvanceRequest> &
PatrolAdvanceObserved() {
  static const func::Lazy<rtk::ActionCreator<FBotPatrolAdvanceRequest>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FBotPatrolAdvanceRequest> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FBotPatrolAdvanceRequest>(
                TEXT("botPosition/patrolAdvanceObserved"));
          });
  return func::eval(Creator);
}

} // namespace BotPositionActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
