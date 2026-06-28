#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Level/LevelTypes.h"
#include "Features/Systems/Landmarks/LandmarkAdapters.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarksAdapters {

struct FLandmarkSeedBuildRequest {
  FLevelTerrainData TerrainData;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

TArray<FLandmark> Build1899LandmarkSeed(
    const FLandmarkSeedBuildRequest &Request);

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
