#include "Features/Systems/Bots/BotActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotActions {

const rtk::ActionCreator<TArray<FBotEntity>> &BotsSeeded() {
  static const rtk::ActionCreator<TArray<FBotEntity>> Creator =
      rtk::createAction<TArray<FBotEntity>>(TEXT("bots/seeded"));
  return Creator;
}

} // namespace BotActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
