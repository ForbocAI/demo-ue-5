#include "Features/Systems/Terrain/TerrainSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainSelectors {

/** User Story: As a features systems terrain consumer, I need to invoke select loaded through a stable signature so the features systems terrain workflow remains explicit and composable. @fn bool SelectLoaded(const FTerrainState &State) */
bool SelectLoaded(const FTerrainState &State) {
  return (func::pipe(State.bTerrainLoaded) |
          [](bool bTerrainLoaded) -> bool { return bTerrainLoaded; })
      .val;
}

} // namespace TerrainSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
