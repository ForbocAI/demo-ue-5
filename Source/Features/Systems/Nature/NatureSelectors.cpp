#include "Features/Systems/Nature/NatureSelectors.h"

#include "Features/Systems/Nature/NatureEntityAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureSelectors {

TArray<FNatureFeatureSeed> SelectAll(const FNatureState &State) {
  return (func::pipe(State.Items) |
         [](const rtk::EntityState<FNatureFeatureSeed> &Items)
             -> TArray<FNatureFeatureSeed> {
    return NatureAdapters::NatureAdapter().getSelectors().selectAll(Items);
  }).val;
}

} // namespace NatureSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
