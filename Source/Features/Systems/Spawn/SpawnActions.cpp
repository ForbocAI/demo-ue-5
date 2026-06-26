#include "Features/Systems/Spawn/SpawnActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace SpawnActions {

const rtk::ActionCreator<FSpawnPointPayload> &PlayerSpawnAnchored() {
  static const rtk::ActionCreator<FSpawnPointPayload> Creator =
      rtk::createAction<FSpawnPointPayload>(TEXT("spawn/playerAnchored"));
  return Creator;
}

} // namespace SpawnActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
