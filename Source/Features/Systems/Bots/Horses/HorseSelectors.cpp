#include "Features/Systems/Bots/Horses/HorseSelectors.h"

#include "Features/Systems/Bots/Horses/HorseAdapters.h"

namespace ForbocAI {
namespace Demo {
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
} // namespace Demo
} // namespace ForbocAI
