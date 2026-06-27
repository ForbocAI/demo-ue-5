#include "Features/Systems/Townspeople/TownspersonSelectors.h"

#include "Features/Systems/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonSelectors {

TArray<FTownspersonSeed> SelectAll(const FTownspersonState &State) {
  return TownspersonAdapters::TownspersonAdapter().getSelectors().selectAll(
      State.Items);
}

} // namespace TownspersonSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
