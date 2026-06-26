#include "Features/Systems/Townspeople/TownspersonFactories.h"

#include "Features/Systems/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonFactories {

FTownspersonState CreateInitialState() {
  FTownspersonState State;
  State.Items = TownspersonAdapters::TownspersonAdapter().getInitialState();
  return State;
}

} // namespace TownspersonFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
