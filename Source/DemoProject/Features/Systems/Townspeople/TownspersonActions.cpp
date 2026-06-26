#include "Features/Systems/Townspeople/TownspersonActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonActions {

const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded() {
  static const rtk::ActionCreator<TArray<FTownspersonSeed>> Creator =
      rtk::createAction<TArray<FTownspersonSeed>>(
          TEXT("townspeople/seeded"));
  return Creator;
}

} // namespace TownspersonActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
