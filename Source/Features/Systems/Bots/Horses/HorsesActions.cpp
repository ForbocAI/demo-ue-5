#include "Features/Systems/Bots/Horses/HorsesActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseActions {

/** User Story: As a systems bots horses consumer, I need to invoke horses seeded through a stable signature so the systems bots horses workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded() */
const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FHorseRouteSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FHorseRouteSeed>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FHorseRouteSeed>>(TEXT("horses/seeded"));
      });
  return func::eval(Creator);
}

} // namespace HorseActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
