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
BuildTerrainMeshPayload(const TerrainReducers::FTerrainMeshBuildRequest &Request) {
  return TerrainReducers::BuildTerrainMeshPayload(Request);
}

} // namespace TerrainSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
