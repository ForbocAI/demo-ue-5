#include "Features/Systems/Bots/SystemsBotsActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotActions {

/** User Story: As a features systems bots consumer, I need to invoke bots seeded through a stable signature so the features systems bots workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotEntity>> &BotsSeeded() */
const rtk::ActionCreator<TArray<FBotEntity>> &BotsSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotEntity>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotEntity>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FBotEntity>>(TEXT("bots/seeded"));
      });
  return func::eval(Creator);
}

/** User Story: As a features systems bots consumer, I need to invoke bot upserted through a stable signature so the features systems bots workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotEntity> &BotUpserted() */
const rtk::ActionCreator<FBotEntity> &BotUpserted() {
  static const func::Lazy<rtk::ActionCreator<FBotEntity>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotEntity> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotEntity>(TEXT("bots/upserted"));
      });
  return func::eval(Creator);
}

} // namespace BotActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
