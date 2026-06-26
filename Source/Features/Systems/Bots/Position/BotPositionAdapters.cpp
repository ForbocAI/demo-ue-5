#include "Features/Systems/Bots/Position/BotPositionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionAdapters {

const rtk::EntityAdapterOps<FBotPositionComponent> &BotPositionAdapter() {
  static const rtk::EntityAdapterOps<FBotPositionComponent> Adapter =
      rtk::createEntityAdapter<FBotPositionComponent>(
          [](const FBotPositionComponent &Position) -> FString {
            return Position.Id;
          });
  return Adapter;
}

} // namespace BotPositionAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
