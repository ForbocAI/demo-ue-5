#include "Features/Systems/Bots/Horses/HorseActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace HorseActions {

const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FHorseRouteSeed>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FHorseRouteSeed>> {
        return rtk::createAction<TArray<FHorseRouteSeed>>(TEXT("horses/seeded"));
      });
  return func::eval(Creator);
}

} // namespace HorseActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
