#include "Features/Systems/Bots/Horses/HorsesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseAdapters {

/** User Story: As a systems bots horses consumer, I need to invoke horse adapter through a stable signature so the systems bots horses workflow remains explicit and composable. @fn const rtk::EntityAdapter<FHorseRouteSeed> &HorseAdapter() */
const rtk::EntityAdapter<FHorseRouteSeed> &HorseAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FHorseRouteSeed>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FHorseRouteSeed> {
        return rtk::createEntityAdapter<FHorseRouteSeed>(
          [](const FHorseRouteSeed &Horse) -> FString { return Horse.Id; });
      });
  return func::eval(Adapter);
}

} // namespace HorseAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
