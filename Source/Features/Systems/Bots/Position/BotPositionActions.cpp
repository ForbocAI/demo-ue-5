#include "Features/Systems/Bots/Position/BotPositionActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionActions {

const rtk::ActionCreator<TArray<FBotPositionComponent>> &
BotPositionsSeeded() {
  static const rtk::ActionCreator<TArray<FBotPositionComponent>> Creator =
      rtk::createAction<TArray<FBotPositionComponent>>(
          TEXT("botPosition/seeded"));
  return Creator;
}

const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved() {
  static const rtk::ActionCreator<FBotPositionMoved> Creator =
      rtk::createAction<FBotPositionMoved>(TEXT("botPosition/moved"));
  return Creator;
}

} // namespace BotPositionActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
