#include "Features/Systems/Bots/Position/Selectors.h"

#include "Features/Systems/Bots/Position/Adapters.h"

namespace ForbocAI {
namespace Game {
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
} // namespace Game
} // namespace ForbocAI
