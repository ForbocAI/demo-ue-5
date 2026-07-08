#include "Features/Systems/Terrain/Selectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainSelectors {

bool SelectLoaded(const FTerrainState &State) {
  return (func::pipe(State.bTerrainLoaded) |
          [](bool bTerrainLoaded) -> bool { return bTerrainLoaded; })
      .val;
}

} // namespace TerrainSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
