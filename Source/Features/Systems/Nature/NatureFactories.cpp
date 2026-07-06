#include "Features/Systems/Nature/NatureFactories.h"

#include "Features/Systems/Nature/NatureEntityAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureFactories {

FNatureState CreateInitialState() {
  FNatureState State;
  State.Items = NatureAdapters::NatureAdapter().getInitialState();
  return State;
}

} // namespace NatureFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
