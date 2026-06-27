#include "Features/Systems/Bots/Horses/HorseFactories.h"

#include "Features/Systems/Bots/Horses/HorseAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace HorseFactories {

FHorseState CreateInitialState() {
  FHorseState State;
  State.Items = HorseAdapters::HorseAdapter().getInitialState();
  return State;
}

} // namespace HorseFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
