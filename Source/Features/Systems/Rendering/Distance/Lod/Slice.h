#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"
#include "Features/Systems/Rendering/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingDistanceLodReducers {

/**
 * @brief Maps authored distance LOD settings into feature-owned stages.
 */
TArray<FLevelDistanceLodStage> ReduceDistanceLodStages(
    const ForbocAI::Game::Data::FRenderingDistanceLodSettings &Settings);

/**
 * @brief Selects the authored LOD stage for one world-space location.
 */
FLevelDistanceLodStage
ReduceDistanceLodStage(const FLevelDistanceLodStageRequest &Request);

} // namespace RenderingDistanceLodReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
