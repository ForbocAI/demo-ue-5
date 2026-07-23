#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Systems/Landmarks/SystemsLandmarksAdapters.h"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarksAdapters {

struct FLandmarkSeedBuildRequest {
  FString RelativeJsonPath;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

/** User Story: As a entities environments landmarks consumer, I need to invoke build landmark seed through a stable signature so the entities environments landmarks workflow remains explicit and composable. @fn TArray<FLandmark> BuildLandmarkSeed( const FLandmarkSeedBuildRequest &Request) */
TArray<FLandmark> BuildLandmarkSeed(
    const FLandmarkSeedBuildRequest &Request);

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
