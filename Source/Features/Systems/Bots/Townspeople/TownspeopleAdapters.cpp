#include "Features/Systems/Bots/Townspeople/TownspeopleAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonAdapters {

/** User Story: As a systems bots townspeople consumer, I need to invoke townsperson adapter through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::EntityAdapter<FTownspersonSeed> &TownspersonAdapter() */
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
