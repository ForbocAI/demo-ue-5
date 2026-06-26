#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotGoalAdapters {

const rtk::EntityAdapterOps<FBotGoalComponent> &BotGoalAdapter() {
  static const rtk::EntityAdapterOps<FBotGoalComponent> Adapter =
      rtk::createEntityAdapter<FBotGoalComponent>(
          [](const FBotGoalComponent &Goals) -> FString { return Goals.Id; });
  return Adapter;
}

} // namespace BotGoalAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
