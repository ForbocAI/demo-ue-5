#include "Features/Systems/Bots/Position/BotPositionSelectors.h"

#include "Features/Systems/Bots/Position/BotPositionAdapters.h"

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

int32 SelectLastInitialPatrolIndex(const FBotPositionState &State) {
  return State.LastInitialPatrolIndex;
}

const FBotInitialPatrolLocationPayload &
SelectLastInitialPatrolLocation(const FBotPositionState &State) {
  return State.LastInitialPatrolLocation;
}

const FBotPatrolAdvancePayload &
SelectLastPatrolAdvance(const FBotPositionState &State) {
  return State.LastPatrolAdvance;
}

} // namespace BotPositionSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
