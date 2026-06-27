#include "Features/Systems/Bots/BotAdapters.h"

namespace ForbocAI {
namespace Demo {
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
} // namespace Demo
} // namespace ForbocAI
