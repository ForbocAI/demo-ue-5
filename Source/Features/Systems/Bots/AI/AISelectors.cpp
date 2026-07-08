#include "Features/Systems/Bots/AI/AISelectors.h"

#include "Features/Systems/Bots/AI/AIAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAISelectors {

TArray<FBotAIComponent> SelectAll(const FBotAIState &State) {
  return BotAIAdapters::BotAIAdapter().getSelectors().selectAll(State.Items);
}

func::Maybe<FBotAIComponent> SelectById(const FBotAIState &State,
                                        const FString &Id) {
  return BotAIAdapters::BotAIAdapter().getSelectors().selectById(State.Items,
                                                                 Id);
}

int32 SelectTotal(const FBotAIState &State) {
  return BotAIAdapters::BotAIAdapter().getSelectors().selectTotal(State.Items);
}

} // namespace BotAISelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
