#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Level/LevelTypes.h"
#include "Features/Systems/Landmarks/LandmarkAdapters.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarksAdapters {

TArray<FLandmark> Build1899LandmarkSeed(
    const FLevelTerrainData &TerrainData);

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
