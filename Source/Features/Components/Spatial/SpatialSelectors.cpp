#include "Features/Components/Spatial/SpatialSelectors.h"

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpatialSelectors {

/** User Story: As a features components spatial consumer, I need to invoke select town lot world units through a stable signature so the features components spatial workflow remains explicit and composable. @fn float SelectTownLotWorldUnits( const ForbocAI::Game::Data::FGeometrySettings &Geometry) */
float SelectTownLotWorldUnits(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return LevelLayoutAdapters::TownLotWorldUnits(Geometry);
}

/** User Story: As a features components spatial consumer, I need to invoke select actor world units from feet through a stable signature so the features components spatial workflow remains explicit and composable. @fn float SelectActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) */
float SelectActorWorldUnitsFromFeet(const FLevelActorFeetRequest &Request) {
  return LevelLayoutAdapters::ActorWorldUnitsFromFeet(Request);
}

} // namespace SpatialSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
