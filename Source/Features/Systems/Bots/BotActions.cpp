#include "Features/Systems/Bots/BotActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotActions {

const rtk::ActionCreator<TArray<FBotEntity>> &BotsSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotEntity>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotEntity>> {
        return rtk::createAction<TArray<FBotEntity>>(TEXT("bots/seeded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotEntity> &BotUpserted() {
  static const func::Lazy<rtk::ActionCreator<FBotEntity>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotEntity> {
        return rtk::createAction<FBotEntity>(TEXT("bots/upserted"));
      });
  return func::eval(Creator);
}

} // namespace BotActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
