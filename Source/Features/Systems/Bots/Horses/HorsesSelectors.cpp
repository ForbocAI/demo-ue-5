#include "Features/Systems/Bots/Horses/HorsesSelectors.h"

#include "Features/Systems/Bots/Horses/HorsesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSelectors {

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
