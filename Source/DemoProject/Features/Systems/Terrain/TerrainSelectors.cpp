#include "Features/Systems/Terrain/TerrainSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TerrainSelectors {

bool SelectLoaded(const FTerrainState &State) {
  return State.bTerrainLoaded;
}

} // namespace TerrainSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
