#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Nature/NatureEntityAdapters.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureAdapters {

struct FNatureSeedBuildRequest {
  FString RelativeJsonPath;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

TArray<FNatureFeatureSeed> BuildNatureSeed(
    const FNatureSeedBuildRequest &Request);

} // namespace NatureAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
