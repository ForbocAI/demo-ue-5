#include "Features/Systems/Bots/SystemsBotsSelectors.h"

#include "Features/Systems/Bots/SystemsBotsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSelectors {

/** User Story: As a features systems bots consumer, I need to invoke select all through a stable signature so the features systems bots workflow remains explicit and composable. @fn TArray<FBotEntity> SelectAll(const FBotState &State) */
TArray<FBotEntity> SelectAll(const FBotState &State) {
  return BotAdapters::BotAdapter().getSelectors().selectAll(State.Items);
}

/** User Story: As a features systems bots consumer, I need to invoke select by id through a stable signature so the features systems bots workflow remains explicit and composable. @fn func::Maybe<FBotEntity> SelectById(const FBotState &State, const FString &Id) */
func::Maybe<FBotEntity> SelectById(const FBotState &State, const FString &Id) {
  return BotAdapters::BotAdapter().getSelectors().selectById(State.Items, Id);
}

/** User Story: As a features systems bots consumer, I need to invoke select total through a stable signature so the features systems bots workflow remains explicit and composable. @fn int32 SelectTotal(const FBotState &State) */
int32 SelectTotal(const FBotState &State) {
  return BotAdapters::BotAdapter().getSelectors().selectTotal(State.Items);
}

} // namespace BotSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
