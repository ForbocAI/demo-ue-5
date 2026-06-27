#include "Features/Systems/Bots/Position/BotPositionSelectors.h"

#include "Features/Systems/Bots/Position/BotPositionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionSelectors {

TArray<FBotPositionComponent> SelectAll(const FBotPositionState &State) {
  return BotPositionAdapters::BotPositionAdapter().getSelectors().selectAll(
      State.Items);
}

func::Maybe<FBotPositionComponent>
SelectById(const FBotPositionState &State, const FString &Id) {
  return BotPositionAdapters::BotPositionAdapter().getSelectors().selectById(
      State.Items, Id);
}

int32 SelectTotal(const FBotPositionState &State) {
  return BotPositionAdapters::BotPositionAdapter().getSelectors().selectTotal(
      State.Items);
}

} // namespace BotPositionSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
