#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Nature/NatureEntityAdapters.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

struct FNatureSeedBuildRequest {
  FString RelativeJsonPath;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

TArray<FNatureFeatureSeed> BuildNatureSeed(
    const FNatureSeedBuildRequest &Request);

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
