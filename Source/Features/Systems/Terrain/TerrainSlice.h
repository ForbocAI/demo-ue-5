#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainReducers {

FTerrainState
ReduceTerrainLoaded(const FTerrainState &State,
                    const rtk::PayloadAction<FLoadedPayload> &Action);

/**
 * Groups the source data needed to build a terrain mesh.
 * Pure data — no behavior.
 */
struct FMeshBuildRequest {
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  const ForbocAI::Game::Data::FGeometrySettings &Geometry;
};

FMeshPayload
BuildTerrainMeshPayload(const FMeshBuildRequest &Request);

} // namespace TerrainReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainSlice {

const rtk::Slice<FTerrainState> &GetSlice();

inline FMeshPayload
BuildTerrainMeshPayload(const TerrainReducers::FMeshBuildRequest &Request) {
  return TerrainReducers::BuildTerrainMeshPayload(Request);
}

} // namespace TerrainSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainFactories {

FTerrainState CreateInitialState();
FLoadedPayload LoadedPayload(const FLoadedSource &Source);

} // namespace TerrainFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
