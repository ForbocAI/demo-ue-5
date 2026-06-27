#include "Features/Systems/Bots/AI/BotAIActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotAIActions {

const rtk::ActionCreator<TArray<FBotAIComponent>> &BotAISeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotAIComponent>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotAIComponent>> {
        return rtk::createAction<TArray<FBotAIComponent>>(TEXT("botAI/seeded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotAIUpdated> &BotAIUpdated() {
  static const func::Lazy<rtk::ActionCreator<FBotAIUpdated>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotAIUpdated> {
        return rtk::createAction<FBotAIUpdated>(TEXT("botAI/updated"));
      });
  return func::eval(Creator);
}

} // namespace BotAIActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
