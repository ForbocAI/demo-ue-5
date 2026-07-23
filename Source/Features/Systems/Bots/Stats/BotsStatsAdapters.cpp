#include "Features/Systems/Bots/Stats/BotsStatsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsAdapters {

/** User Story: As a systems bots stats consumer, I need to invoke bot stats adapter through a stable signature so the systems bots stats workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotStatsComponent> &BotStatsAdapter() */
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
