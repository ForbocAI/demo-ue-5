#include "Features/Systems/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonAdapters {

const rtk::EntityAdapterOps<FTownspersonSeed> &TownspersonAdapter() {
  static const rtk::EntityAdapterOps<FTownspersonSeed> Adapter =
      rtk::createEntityAdapter<FTownspersonSeed>(
          [](const FTownspersonSeed &Townsperson) -> FString {
            return Townsperson.Id;
          });
  return Adapter;
}

} // namespace TownspersonAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
