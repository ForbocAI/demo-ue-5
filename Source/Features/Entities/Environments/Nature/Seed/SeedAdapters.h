#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Nature/Entity/EntityAdapters.h"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

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
