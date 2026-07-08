#include "Features/Systems/Bots/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAdapters {

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
