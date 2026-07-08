#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Level/Types.h"
#include "Features/Systems/Terrain/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainReducers {

FTerrainState
ReduceTerrainLoaded(const FTerrainState &State,
                    const rtk::PayloadAction<FTerrainLoadedPayload> &Action);

/**
 * Groups the source data needed to build a terrain mesh.
 * Pure data — no behavior.
 */
struct FTerrainMeshBuildRequest {
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry;
};

FTerrainMeshPayload
BuildTerrainMeshPayload(const FTerrainMeshBuildRequest &Request);

} // namespace TerrainReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Terrain/Actions.h"
#include "Features/Systems/Terrain/Selectors.h"
#include "Features/Systems/Terrain/Types.h"

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

#include "Features/Systems/Terrain/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainFactories {

FTerrainState CreateInitialState();
FTerrainLoadedPayload LoadedPayload(const FTerrainLoadedSource &Source);

} // namespace TerrainFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
