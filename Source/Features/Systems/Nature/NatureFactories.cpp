#include "Features/Systems/Nature/NatureFactories.h"

#include "Features/Systems/Nature/NatureEntityAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureFactories {

FNatureState CreateInitialState() {
  FNatureState State;
  State.Items = NatureAdapters::NatureAdapter().getInitialState();
  return State;
}

} // namespace NatureFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
