#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Systems/Terrain/SystemsTerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainReducers {

/** User Story: As a features systems terrain consumer, I need to invoke reduce terrain loaded through a stable signature so the features systems terrain workflow remains explicit and composable. @fn FTerrainState ReduceTerrainLoaded(const FTerrainState &State, const rtk::PayloadAction<FLoadedPayload> &Action) */
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

/** User Story: As a features systems terrain consumer, I need to invoke build terrain mesh payload through a stable signature so the features systems terrain workflow remains explicit and composable. @fn FMeshPayload BuildTerrainMeshPayload(const FMeshBuildRequest &Request) */
FMeshPayload
BuildTerrainMeshPayload(const FMeshBuildRequest &Request);

} // namespace TerrainReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Features/Systems/Terrain/SystemsTerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainSlice {

/** User Story: As a features systems terrain consumer, I need to invoke get slice through a stable signature so the features systems terrain workflow remains explicit and composable. @fn const rtk::Slice<FTerrainState> &GetSlice() */
const rtk::Slice<FTerrainState> &GetSlice();

/** User Story: As a features systems terrain consumer, I need to invoke build terrain mesh payload through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline FMeshPayload BuildTerrainMeshPayload(const TerrainReducers::FMeshBuildRequest &Request) */
inline FMeshPayload
BuildTerrainMeshPayload(const TerrainReducers::FMeshBuildRequest &Request) {
  return TerrainReducers::BuildTerrainMeshPayload(Request);
}

} // namespace TerrainSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Terrain/SystemsTerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainFactories {

/** User Story: As a features systems terrain consumer, I need to invoke create initial state through a stable signature so the features systems terrain workflow remains explicit and composable. @fn FTerrainState CreateInitialState() */
FTerrainState CreateInitialState();
/** User Story: As a features systems terrain consumer, I need to invoke loaded payload through a stable signature so the features systems terrain workflow remains explicit and composable. @fn FLoadedPayload LoadedPayload(const FLoadedSource &Source) */
FLoadedPayload LoadedPayload(const FLoadedSource &Source);

} // namespace TerrainFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
