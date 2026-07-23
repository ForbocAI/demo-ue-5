#include "Features/Systems/Bots/Stats/BotsStatsSelectors.h"

#include "Features/Systems/Bots/Stats/BotsStatsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsSelectors {

/** User Story: As a systems bots stats consumer, I need to invoke select all through a stable signature so the systems bots stats workflow remains explicit and composable. @fn TArray<FBotStatsComponent> SelectAll(const FBotStatsState &State) */
TArray<FBotStatsComponent> SelectAll(const FBotStatsState &State) {
  return BotStatsAdapters::BotStatsAdapter().getSelectors().selectAll(
      State.Items);
}

/** User Story: As a systems bots stats consumer, I need to invoke select by id through a stable signature so the systems bots stats workflow remains explicit and composable. @fn func::Maybe<FBotStatsComponent> SelectById(const FBotStatsState &State, const FString &Id) */
func::Maybe<FBotStatsComponent>
SelectById(const FBotStatsState &State, const FString &Id) {
  return BotStatsAdapters::BotStatsAdapter().getSelectors().selectById(
      State.Items, Id);
}

/** User Story: As a systems bots stats consumer, I need to invoke select total through a stable signature so the systems bots stats workflow remains explicit and composable. @fn int32 SelectTotal(const FBotStatsState &State) */
int32 SelectTotal(const FBotStatsState &State) {
  return BotStatsAdapters::BotStatsAdapter().getSelectors().selectTotal(
      State.Items);
}

} // namespace BotStatsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
