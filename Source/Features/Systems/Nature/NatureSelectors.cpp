#include "Features/Systems/Nature/NatureSelectors.h"

#include "Features/Systems/Nature/NatureAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureSelectors {

TArray<FNatureFeatureSeed> SelectAll(const FNatureState &State) {
  return NatureAdapters::NatureAdapter().getSelectors().selectAll(
      State.Items);
}

} // namespace NatureSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
