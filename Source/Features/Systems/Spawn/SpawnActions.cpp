#include "Features/Systems/Spawn/SpawnActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpawnActions {

const rtk::ActionCreator<FSpawnPointPayload> &PlayerSpawnAnchored() {
  static const func::Lazy<rtk::ActionCreator<FSpawnPointPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSpawnPointPayload> {
        return rtk::createAction<FSpawnPointPayload>(TEXT("spawn/playerAnchored"));
      });
  return func::eval(Creator);
}

} // namespace SpawnActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
