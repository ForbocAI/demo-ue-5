#include "Features/Systems/Terrain/TerrainSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainSelectors {

bool SelectLoaded(const FTerrainState &State) {
  return (func::pipe(State.bTerrainLoaded) |
          [](bool bTerrainLoaded) -> bool { return bTerrainLoaded; })
      .val;
}

} // namespace TerrainSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
