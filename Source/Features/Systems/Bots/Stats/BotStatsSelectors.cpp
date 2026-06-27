#include "Features/Systems/Bots/Stats/BotStatsSelectors.h"

#include "Features/Systems/Bots/Stats/BotStatsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsSelectors {

TArray<FBotStatsComponent> SelectAll(const FBotStatsState &State) {
  return BotStatsAdapters::BotStatsAdapter().getSelectors().selectAll(
      State.Items);
}

func::Maybe<FBotStatsComponent>
SelectById(const FBotStatsState &State, const FString &Id) {
  return BotStatsAdapters::BotStatsAdapter().getSelectors().selectById(
      State.Items, Id);
}

int32 SelectTotal(const FBotStatsState &State) {
  return BotStatsAdapters::BotStatsAdapter().getSelectors().selectTotal(
      State.Items);
}

} // namespace BotStatsSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
