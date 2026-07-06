#include "Features/Systems/Spawn/SpawnActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnActions {

const rtk::ActionCreator<FSpawnPointPayload> &PlayerSpawnAnchored() {
  static const func::Lazy<rtk::ActionCreator<FSpawnPointPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSpawnPointPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FSpawnPointPayload>(TEXT("spawn/playerAnchored"));
      });
  return func::eval(Creator);
}

} // namespace SpawnActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
