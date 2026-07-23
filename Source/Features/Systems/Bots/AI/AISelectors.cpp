#include "Features/Systems/Bots/AI/AISelectors.h"

#include "Features/Systems/Bots/AI/AIAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAISelectors {

/** User Story: As a systems bots ai consumer, I need to invoke select all through a stable signature so the systems bots ai workflow remains explicit and composable. @fn TArray<FBotAIComponent> SelectAll(const FBotAIState &State) */
TArray<FBotAIComponent> SelectAll(const FBotAIState &State) {
  return BotAIAdapters::BotAIAdapter().getSelectors().selectAll(State.Items);
}

/** User Story: As a systems bots ai consumer, I need to invoke select by id through a stable signature so the systems bots ai workflow remains explicit and composable. @fn func::Maybe<FBotAIComponent> SelectById(const FBotAIState &State, const FString &Id) */
func::Maybe<FBotAIComponent> SelectById(const FBotAIState &State,
                                        const FString &Id) {
  return BotAIAdapters::BotAIAdapter().getSelectors().selectById(State.Items,
                                                                 Id);
}

/** User Story: As a systems bots ai consumer, I need to invoke select total through a stable signature so the systems bots ai workflow remains explicit and composable. @fn int32 SelectTotal(const FBotAIState &State) */
int32 SelectTotal(const FBotAIState &State) {
  return BotAIAdapters::BotAIAdapter().getSelectors().selectTotal(State.Items);
}

} // namespace BotAISelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
