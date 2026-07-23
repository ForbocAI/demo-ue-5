#include "Features/Systems/Nature/SystemsNatureSelectors.h"

#include "Features/Systems/Nature/Entity/EntityAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureSelectors {

/** User Story: As a features systems nature consumer, I need to invoke select all through a stable signature so the features systems nature workflow remains explicit and composable. @fn TArray<FFeatureSeed> SelectAll(const FNatureState &State) */
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
