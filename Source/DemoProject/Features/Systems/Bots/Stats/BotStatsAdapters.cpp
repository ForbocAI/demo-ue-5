#include "Features/Systems/Bots/Stats/BotStatsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsAdapters {

const rtk::EntityAdapterOps<FBotStatsComponent> &BotStatsAdapter() {
  static const rtk::EntityAdapterOps<FBotStatsComponent> Adapter =
      rtk::createEntityAdapter<FBotStatsComponent>(
          [](const FBotStatsComponent &Stats) -> FString { return Stats.Id; });
  return Adapter;
}

} // namespace BotStatsAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
