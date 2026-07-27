#include "Features/Systems/Bots/Stats/StatsActions.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsActions {

/** User Story: As a systems bots stats consumer, I need to invoke bot stats seeded through a stable signature so the systems bots stats workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded() */
const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotStatsComponent>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotStatsComponent>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FBotStatsComponent>>(
          TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV16E74F1FA819));
      });
  return func::eval(Creator);
}

/** User Story: As a systems bots stats consumer, I need to invoke bot stats updated through a stable signature so the systems bots stats workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated() */
const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated() {
  static const func::Lazy<rtk::ActionCreator<FBotStatsUpdate>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotStatsUpdate> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotStatsUpdate>(TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGVCA8600DD7ECB));
      });
  return func::eval(Creator);
}

} // namespace BotStatsActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
