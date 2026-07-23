#include "Features/Systems/Bots/SystemsBotsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAdapters {

/** User Story: As a features systems bots consumer, I need to invoke bot adapter through a stable signature so the features systems bots workflow remains explicit and composable. @fn const rtk::EntityAdapter<FBotEntity> &BotAdapter() */
const rtk::EntityAdapter<FBotEntity> &BotAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FBotEntity>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FBotEntity> {
        return rtk::createEntityAdapter<FBotEntity>(
          [](const FBotEntity &Bot) -> FString { return Bot.Id; });
      });
  return func::eval(Adapter);
}

} // namespace BotAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
