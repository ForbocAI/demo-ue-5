#include "Features/Systems/Bots/Horses/HorsesSelectors.h"

#include "Features/Systems/Bots/Horses/HorsesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSelectors {

/** User Story: As a systems bots horses consumer, I need to invoke select all through a stable signature so the systems bots horses workflow remains explicit and composable. @fn TArray<FHorseRouteSeed> SelectAll(const FHorseState &State) */
TArray<FHorseRouteSeed> SelectAll(const FHorseState &State) {
  return (func::pipe(State.Items) |
         [](const rtk::EntityState<FHorseRouteSeed> &Items)
             -> TArray<FHorseRouteSeed> {
    return HorseAdapters::HorseAdapter().getSelectors().selectAll(Items);
  }).val;
}

} // namespace HorseSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
