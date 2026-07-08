#include "Features/Systems/Bots/Stats/StatsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsAdapters {

const rtk::EntityAdapter<FBotStatsComponent> &BotStatsAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FBotStatsComponent>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FBotStatsComponent> {
        return rtk::createEntityAdapter<FBotStatsComponent>(
          [](const FBotStatsComponent &Stats) -> FString { return Stats.Id; });
      });
  return func::eval(Adapter);
}

} // namespace BotStatsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
