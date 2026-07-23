#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Nature/Entity/EntityAdapters.h"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

struct FBuildRequest {
  FString RelativeJsonPath;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

/** User Story: As a environments nature seed consumer, I need to invoke build nature seed through a stable signature so the environments nature seed workflow remains explicit and composable. @fn TArray<FFeatureSeed> BuildNatureSeed( const FBuildRequest &Request) */
TArray<FFeatureSeed> BuildNatureSeed(
    const FBuildRequest &Request);

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
