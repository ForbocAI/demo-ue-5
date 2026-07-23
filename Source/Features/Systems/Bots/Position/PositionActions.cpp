#include "Features/Systems/Bots/Position/PositionActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionActions {

/** User Story: As a systems bots position consumer, I need to invoke bot positions seeded through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotPositionComponent>> & BotPositionsSeeded() */
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

/** User Story: As a systems bots position consumer, I need to invoke bot position upserted through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotPositionComponent> &BotPositionUpserted() */
const rtk::ActionCreator<FBotPositionComponent> &BotPositionUpserted() {
  static const func::Lazy<rtk::ActionCreator<FBotPositionComponent>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotPositionComponent> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotPositionComponent>(
            TEXT("botPosition/upserted"));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots position consumer, I need to invoke bot position moved through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved() */
const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved() {
  static const func::Lazy<rtk::ActionCreator<FBotPositionMoved>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotPositionMoved> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotPositionMoved>(TEXT("botPosition/moved"));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots position consumer, I need to invoke initial patrol observed through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotInitialPatrolLocationRequest> & InitialPatrolObserved() */
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

/** User Story: As a systems bots position consumer, I need to invoke patrol advance observed through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotPatrolAdvanceRequest> & PatrolAdvanceObserved() */
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
