#include "Features/Systems/Bots/Horses/HorseAdapters.h"

namespace ForbocAI {
namespace Demo {
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
} // namespace Demo
} // namespace ForbocAI
