#include "Features/Systems/Bots/AI/AIAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIAdapters {

const rtk::EntityAdapter<FBotAIComponent> &BotAIAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FBotAIComponent>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FBotAIComponent> {
        return rtk::createEntityAdapter<FBotAIComponent>(
          [](const FBotAIComponent &AI) -> FString { return AI.Id; });
      });
  return func::eval(Adapter);
}

} // namespace BotAIAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
