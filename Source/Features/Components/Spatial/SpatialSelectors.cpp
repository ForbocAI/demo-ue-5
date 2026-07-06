#include "Features/Components/Spatial/SpatialSelectors.h"

#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpatialSelectors {

float SelectTownLotWorldUnits(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return LevelLayoutSlice::TownLotWorldUnits(Geometry);
}

float SelectActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) {
  return LevelLayoutSlice::ActorWorldUnitsFromFeet(Request);
}

} // namespace SpatialSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
