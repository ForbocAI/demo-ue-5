#include "Features/Systems/Bots/Townspeople/TownspersonFactories.h"

#include "Features/Systems/Bots/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonFactories {

FTownspersonState CreateInitialState() {
  FTownspersonState State;
  State.Items = TownspersonAdapters::TownspersonAdapter().getInitialState();
  return State;
}

} // namespace TownspersonFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
