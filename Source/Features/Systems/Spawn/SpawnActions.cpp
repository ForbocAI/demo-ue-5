#include "Features/Systems/Spawn/SpawnActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnActions {

/** User Story: As a features systems spawn consumer, I need to invoke player spawn anchored through a stable signature so the features systems spawn workflow remains explicit and composable. @fn const rtk::ActionCreator<FPointPayload> &PlayerSpawnAnchored() */
const rtk::ActionCreator<FPointPayload> &PlayerSpawnAnchored() {
  static const func::Lazy<rtk::ActionCreator<FPointPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FPointPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FPointPayload>(TEXT("spawn/playerAnchored"));
      });
  return func::eval(Creator);
}

} // namespace SpawnActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
