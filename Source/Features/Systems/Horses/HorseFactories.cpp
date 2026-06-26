#include "Features/Systems/Horses/HorseFactories.h"

#include "Features/Systems/Horses/HorseAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseFactories {

FHorseState CreateInitialState() {
  FHorseState State;
  State.Items = HorseAdapters::HorseAdapter().getInitialState();
  return State;
}

} // namespace HorseFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
