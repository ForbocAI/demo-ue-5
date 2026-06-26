#include "Features/Systems/Bots/BotAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAdapters {

const rtk::EntityAdapterOps<FBotEntity> &BotAdapter() {
  static const rtk::EntityAdapterOps<FBotEntity> Adapter =
      rtk::createEntityAdapter<FBotEntity>(
          [](const FBotEntity &Bot) -> FString { return Bot.Id; });
  return Adapter;
}

} // namespace BotAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
