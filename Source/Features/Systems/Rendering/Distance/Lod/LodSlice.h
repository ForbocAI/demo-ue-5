#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingDistanceLodReducers {

/**
 * @brief Maps authored distance LOD settings into feature-owned stages.
 */
TArray<FLevelDistanceLodStage> ReduceDistanceLodStages(
    const ForbocAI::Game::Data::FLodSettings &Settings);

/**
 * @brief Selects the authored LOD stage for one world-space location.
 */
FLevelDistanceLodStage
ReduceDistanceLodStage(const FLevelDistanceLodStageRequest &Request);

} // namespace RenderingDistanceLodReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
