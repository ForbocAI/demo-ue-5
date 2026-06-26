#include "Features/Systems/Bots/AI/BotAIAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAIAdapters {

const rtk::EntityAdapterOps<FBotAIComponent> &BotAIAdapter() {
  static const rtk::EntityAdapterOps<FBotAIComponent> Adapter =
      rtk::createEntityAdapter<FBotAIComponent>(
          [](const FBotAIComponent &AI) -> FString { return AI.Id; });
  return Adapter;
}

} // namespace BotAIAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
