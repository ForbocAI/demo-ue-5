#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonActions {

const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FTownspersonSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FTownspersonSeed>> {
        return rtk::createAction<TArray<FTownspersonSeed>>(
          TEXT("townspeople/seeded"));
      });
  return func::eval(Creator);
}

} // namespace TownspersonActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
