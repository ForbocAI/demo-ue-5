#include "Features/Systems/Bots/BotSelectors.h"

#include "Features/Systems/Bots/BotAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotSelectors {

TArray<FBotEntity> SelectAll(const FBotState &State) {
  return BotAdapters::BotAdapter().getSelectors().selectAll(State.Items);
}

func::Maybe<FBotEntity> SelectById(const FBotState &State, const FString &Id) {
  return BotAdapters::BotAdapter().getSelectors().selectById(State.Items, Id);
}

int32 SelectTotal(const FBotState &State) {
  return BotAdapters::BotAdapter().getSelectors().selectTotal(State.Items);
}

} // namespace BotSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
