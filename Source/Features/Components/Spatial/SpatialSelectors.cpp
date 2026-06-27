#include "Features/Components/Spatial/SpatialSelectors.h"

#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpatialSelectors {

float SelectTownLotWorldUnits() {
  return LevelLayoutSlice::TownLotWorldUnits();
}

float SelectActorWorldUnitsFromFeet(float Feet) {
  return LevelLayoutSlice::ActorWorldUnitsFromFeet(Feet);
}

} // namespace SpatialSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
