#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/Terrain/TerrainReducers.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainSlice {

const rtk::Slice<FTerrainState> &GetSlice();

inline FTerrainMeshPayload
BuildTerrainMeshPayload(const FLevelTerrainData &TerrainData,
                        const FLevelOrthoData &OrthoData,
                        const ForbocAI::Game::Data::FLevelGeometrySettings
                            &Geometry) {
  return TerrainReducers::BuildTerrainMeshPayload(TerrainData, OrthoData,
                                                  Geometry);
}

} // namespace TerrainSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
