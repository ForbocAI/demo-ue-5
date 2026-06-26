#pragma once

#include "CoreMinimal.h"
#include "FrenchGulch/Components/FrenchGulchTerrainData.h"
#include "FrenchGulch/Systems/FrenchGulchSlice.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {

namespace FrenchGulchTown {
TArray<FFrenchGulchLandmark>
Build1899LandmarkSeed(const FFrenchGulchTerrainData &TerrainData);
} // namespace FrenchGulchTown

} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
