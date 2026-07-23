#include "Features/Systems/Bots/Goals/GoalsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalAdapters {

/** User Story: As a systems bots goals consumer, I need to invoke bot goal adapter through a stable signature so the systems bots goals workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotGoalComponent> &BotGoalAdapter() */
const rtk::EntityAdapter<FBotGoalComponent> &BotGoalAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FBotGoalComponent>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FBotGoalComponent> {
        return rtk::createEntityAdapter<FBotGoalComponent>(
          [](const FBotGoalComponent &Goals) -> FString { return Goals.Id; });
      });
  return func::eval(Adapter);
}

} // namespace BotGoalAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
