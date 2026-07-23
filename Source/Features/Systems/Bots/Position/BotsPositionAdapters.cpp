#include "Features/Systems/Bots/Position/PositionAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionAdapters {

/** User Story: As a systems bots position consumer, I need to invoke bot position adapter through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotPositionComponent> &BotPositionAdapter() */
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
