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
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

TArray<FLandmark> BuildLandmarkSeed(
    const FLandmarkSeedBuildRequest &Request);

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
