#include "Features/Systems/Bots/Stats/StatsActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsActions {

const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotStatsComponent>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotStatsComponent>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FBotStatsComponent>>(
          TEXT("botStats/seeded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated() {
  static const func::Lazy<rtk::ActionCreator<FBotStatsUpdate>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotStatsUpdate> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotStatsUpdate>(TEXT("botStats/updated"));
      });
  return func::eval(Creator);
}

} // namespace BotStatsActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
