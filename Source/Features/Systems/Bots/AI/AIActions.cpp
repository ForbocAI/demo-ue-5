#include "Features/Systems/Bots/AI/AIActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIActions {

/** User Story: As a systems bots ai consumer, I need to invoke bot aiseeded through a stable signature so the systems bots ai workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotAIComponent>> &BotAISeeded() */
const rtk::ActionCreator<TArray<FBotAIComponent>> &BotAISeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotAIComponent>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotAIComponent>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FBotAIComponent>>(TEXT("botAI/seeded"));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots ai consumer, I need to invoke bot aiupdated through a stable signature so the systems bots ai workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotAIUpdated> &BotAIUpdated() */
const rtk::ActionCreator<FBotAIUpdated> &BotAIUpdated() {
  static const func::Lazy<rtk::ActionCreator<FBotAIUpdated>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotAIUpdated> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotAIUpdated>(TEXT("botAI/updated"));
      });
  return func::eval(Creator);
}

} // namespace BotAIActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
