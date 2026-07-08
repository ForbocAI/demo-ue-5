#include "Features/Systems/Nature/Selectors.h"

#include "Features/Systems/Nature/Entity/Adapters.h"

namespace ForbocAI {
namespace Game {
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
} // namespace Game
} // namespace ForbocAI
