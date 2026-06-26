#include "Features/Systems/Horses/HorseSelectors.h"

#include "Features/Systems/Horses/HorseAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseSelectors {

TArray<FHorseRouteSeed> SelectAll(const FHorseState &State) {
  return HorseAdapters::HorseAdapter().getSelectors().selectAll(State.Items);
}

} // namespace HorseSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
