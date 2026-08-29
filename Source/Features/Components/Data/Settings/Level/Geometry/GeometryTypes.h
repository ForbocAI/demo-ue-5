#pragma once

#include "Features/Components/Data/Settings/Level/Geometry/Block/Rendering/BlockRenderingTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Labels/GeometryLabelsTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Lifecycle/GeometryLifecycleTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Patrol/Route/RouteTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Player/Spawn/PlayerSpawnTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Terrain/Extent/ExtentTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Terrain/Sampling/SamplingTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Terrain/Mesh/TerrainMeshTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Terrain/Quad/QuadTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/Town/Layout/TownLayoutTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/World/Scale/ScaleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FGeometrySettings {
  FExtentSettings TerrainExtent;
  FTownLayoutSettings TownLayout;
  FScaleSettings WorldScale;
  FSpawnGeometrySettings PlayerSpawn;
  FLabelGeometrySettings Labels;
  FSamplingSettings TerrainSampling;
  FQuadSettings TerrainQuad;
  FMeshSettings TerrainMesh;
  FBlockRenderSettings BlockRender;
  FLifecycleSettings Lifecycle;
  FRouteGeometrySettings PatrolRoute;
};

/** User Story: As a level geometry consumer, I need to compare values for equality through a stable signature so geometry settings remain explicit and composable. @fn inline bool operator==(const FGeometrySettings &Left, const FGeometrySettings &Right) */
inline bool operator==(const FGeometrySettings &Left,
                       const FGeometrySettings &Right) {
  return Left.TerrainExtent == Right.TerrainExtent &&
         Left.TownLayout == Right.TownLayout &&
         Left.WorldScale == Right.WorldScale &&
         Left.PlayerSpawn == Right.PlayerSpawn && Left.Labels == Right.Labels &&
         Left.TerrainSampling == Right.TerrainSampling &&
         Left.TerrainQuad == Right.TerrainQuad &&
         Left.TerrainMesh == Right.TerrainMesh &&
         Left.BlockRender == Right.BlockRender &&
         Left.Lifecycle == Right.Lifecycle &&
         Left.PatrolRoute == Right.PatrolRoute;
}

/** User Story: As a level geometry consumer, I need to compare values for inequality through a stable signature so geometry settings remain explicit and composable. @fn inline bool operator!=(const FGeometrySettings &Left, const FGeometrySettings &Right) */
inline bool operator!=(const FGeometrySettings &Left,
                       const FGeometrySettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
