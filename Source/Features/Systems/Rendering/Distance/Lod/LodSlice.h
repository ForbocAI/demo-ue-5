#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingDistanceLodReducers {

/**
 * @fn TArray<FLevelDistanceLodStage> ReduceDistanceLodStages( const ForbocAI::Game::Data::FLodSettings &Settings)
 * @brief Maps authored distance LOD settings into feature-owned stages.
 * User Story: As a rendering distance lod consumer, I need to invoke reduce distance lod stages through a stable signature so the rendering distance lod workflow remains explicit and composable.
 */
TArray<FLevelDistanceLodStage> ReduceDistanceLodStages(
    const ForbocAI::Game::Data::FLodSettings &Settings);

/**
 * @fn FLevelDistanceLodStage ReduceDistanceLodStage(const FLevelDistanceLodStageRequest &Request)
 * @brief Selects the authored LOD stage for one world-space location.
 * User Story: As a rendering distance lod consumer, I need to invoke reduce distance lod stage through a stable signature so the rendering distance lod workflow remains explicit and composable.
 */
FLevelDistanceLodStage
ReduceDistanceLodStage(const FLevelDistanceLodStageRequest &Request);

} // namespace RenderingDistanceLodReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
