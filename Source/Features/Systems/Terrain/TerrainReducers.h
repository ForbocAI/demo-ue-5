#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Level/LevelTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainReducers {

FTerrainState
ReduceTerrainLoaded(const FTerrainState &State,
                    const rtk::PayloadAction<FTerrainLoadedPayload> &Action);

FTerrainMeshPayload
BuildTerrainMeshPayload(const FLevelTerrainData &TerrainData,
                        const FLevelOrthoData &OrthoData,
                        const ForbocAI::Game::Data::FLevelGeometrySettings
                            &Geometry);

} // namespace TerrainReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
