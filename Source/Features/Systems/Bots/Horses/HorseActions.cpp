#include "Features/Systems/Horses/HorseActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseActions {

const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded() {
  static const rtk::ActionCreator<TArray<FHorseRouteSeed>> Creator =
      rtk::createAction<TArray<FHorseRouteSeed>>(TEXT("horses/seeded"));
  return Creator;
}

} // namespace HorseActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
