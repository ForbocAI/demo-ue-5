#include "Features/Systems/Bots/Position/BotPositionActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionActions {

const rtk::ActionCreator<TArray<FBotPositionComponent>> &
BotPositionsSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotPositionComponent>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotPositionComponent>> {
        return rtk::createAction<TArray<FBotPositionComponent>>(
          TEXT("botPosition/seeded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotPositionComponent> &BotPositionUpserted() {
  static const func::Lazy<rtk::ActionCreator<FBotPositionComponent>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotPositionComponent> {
        return rtk::createAction<FBotPositionComponent>(
            TEXT("botPosition/upserted"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved() {
  static const func::Lazy<rtk::ActionCreator<FBotPositionMoved>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotPositionMoved> {
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
            return rtk::createAction<FBotPatrolAdvanceRequest>(
                TEXT("botPosition/patrolAdvanceObserved"));
          });
  return func::eval(Creator);
}

} // namespace BotPositionActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
