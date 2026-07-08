#include "Features/Components/Spatial/SpatialSelectors.h"

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpatialSelectors {

float SelectTownLotWorldUnits(
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return LevelLayoutAdapters::TownLotWorldUnits(Geometry);
}

float SelectActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) {
  return LevelLayoutAdapters::ActorWorldUnitsFromFeet(Request);
}

} // namespace SpatialSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
