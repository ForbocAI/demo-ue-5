#include "Features/Systems/Bots/Stats/BotStatsActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotStatsActions {

const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FBotStatsComponent>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FBotStatsComponent>> {
        return rtk::createAction<TArray<FBotStatsComponent>>(
          TEXT("botStats/seeded"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated() {
  static const func::Lazy<rtk::ActionCreator<FBotStatsUpdate>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FBotStatsUpdate> {
        return rtk::createAction<FBotStatsUpdate>(TEXT("botStats/updated"));
      });
  return func::eval(Creator);
}

} // namespace BotStatsActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
