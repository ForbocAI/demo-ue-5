#include "Features/Systems/Bots/AI/BotAIActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAIActions {

const rtk::ActionCreator<TArray<FBotAIComponent>> &BotAISeeded() {
  static const rtk::ActionCreator<TArray<FBotAIComponent>> Creator =
      rtk::createAction<TArray<FBotAIComponent>>(TEXT("botAI/seeded"));
  return Creator;
}

const rtk::ActionCreator<FBotAIUpdated> &BotAIUpdated() {
  static const rtk::ActionCreator<FBotAIUpdated> Creator =
      rtk::createAction<FBotAIUpdated>(TEXT("botAI/updated"));
  return Creator;
}

} // namespace BotAIActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
