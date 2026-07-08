#include "Features/Systems/Nature/SystemsNatureSelectors.h"

#include "Features/Systems/Nature/Entity/EntityAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureSelectors {

TArray<FFeatureSeed> SelectAll(const FNatureState &State) {
  return (func::pipe(State.Items) |
         [](const rtk::EntityState<FFeatureSeed> &Items)
             -> TArray<FFeatureSeed> {
    return NatureAdapters::NatureAdapter().getSelectors().selectAll(Items);
  }).val;
}

} // namespace NatureSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
