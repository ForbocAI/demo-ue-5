#include "Features/Systems/Horses/HorseAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseAdapters {

const rtk::EntityAdapterOps<FHorseRouteSeed> &HorseAdapter() {
  static const rtk::EntityAdapterOps<FHorseRouteSeed> Adapter =
      rtk::createEntityAdapter<FHorseRouteSeed>(
          [](const FHorseRouteSeed &Horse) -> FString { return Horse.Id; });
  return Adapter;
}

} // namespace HorseAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
