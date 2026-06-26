#include "Features/Systems/Bots/Stats/BotStatsActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsActions {

const rtk::ActionCreator<TArray<FBotStatsComponent>> &BotStatsSeeded() {
  static const rtk::ActionCreator<TArray<FBotStatsComponent>> Creator =
      rtk::createAction<TArray<FBotStatsComponent>>(
          TEXT("botStats/seeded"));
  return Creator;
}

const rtk::ActionCreator<FBotStatsUpdate> &BotStatsUpdated() {
  static const rtk::ActionCreator<FBotStatsUpdate> Creator =
      rtk::createAction<FBotStatsUpdate>(TEXT("botStats/updated"));
  return Creator;
}

} // namespace BotStatsActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
