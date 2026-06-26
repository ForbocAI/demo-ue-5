#include "Features/Systems/Nature/NatureFactories.h"

#include "Features/Systems/Nature/NatureAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureFactories {

FNatureState CreateInitialState() {
  FNatureState State;
  State.Items = NatureAdapters::NatureAdapter().getInitialState();
  return State;
}

} // namespace NatureFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
