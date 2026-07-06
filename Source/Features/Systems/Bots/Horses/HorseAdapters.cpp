#include "Features/Systems/Bots/Horses/HorseAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseAdapters {

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
