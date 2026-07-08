#include "Features/Systems/Bots/Townspeople/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonAdapters {

const rtk::EntityAdapter<FTownspersonSeed> &TownspersonAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FTownspersonSeed>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FTownspersonSeed> {
        return rtk::createEntityAdapter<FTownspersonSeed>(
          [](const FTownspersonSeed &Townsperson) -> FString {
            return Townsperson.Id;
          });
      });
  return func::eval(Adapter);
}

} // namespace TownspersonAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
