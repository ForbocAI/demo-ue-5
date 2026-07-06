#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Level/LevelTypes.h"
#include "Features/Systems/Landmarks/LandmarkAdapters.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

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
