#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalAdapters {

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
