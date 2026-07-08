#include "Features/Systems/Bots/Position/PositionAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionAdapters {

const rtk::EntityAdapter<FBotPositionComponent> &BotPositionAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FBotPositionComponent>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FBotPositionComponent> {
        return rtk::createEntityAdapter<FBotPositionComponent>(
          [](const FBotPositionComponent &Position) -> FString {
            return Position.Id;
          });
      });
  return func::eval(Adapter);
}

} // namespace BotPositionAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
