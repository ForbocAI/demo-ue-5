#pragma once

#include "CoreMinimal.h"
#include "Features/Components/TerrainData.h"
#include "Features/Systems/MapSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

namespace TownLandmarks {
TArray<FMapLandmark>
Build1899LandmarkSeed(const FMapTerrainData &TerrainData);
} // namespace TownLandmarks

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
