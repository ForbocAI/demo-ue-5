#include "Features/Systems/Rendering/Distance/Lod/LodSlice.h"

#include "Core/fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingDistanceLodReducers {
namespace {

/** User Story: As a rendering distance lod consumer, I need to invoke reduce distance lod stage settings through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn FLevelDistanceLodStage ReduceDistanceLodStageSettings( const ForbocAI::Game::Data::FStageSettings &Settings) */
FLevelDistanceLodStage ReduceDistanceLodStageSettings(
    const ForbocAI::Game::Data::FStageSettings &Settings) {
  return {{Settings.Selection.Id, Settings.Selection.MaxDistance},
          {Settings.Mesh.StaticMeshForcedLodModel,
           Settings.Mesh.SkeletalMeshForcedLodModel,
           Settings.Mesh.SkeletalMeshMinLodModel},
          {Settings.Timing.CullDistance,
           Settings.Timing.ActorTickIntervalSeconds},
          {Settings.Visibility.bStaticVisible,
           Settings.Visibility.bDynamicVisible,
           Settings.Visibility.bLabelsVisible},
          {Settings.Behavior.bAnimated,
           Settings.Behavior.bUpdateRateOptimizationsEnabled,
           Settings.Behavior.bPatrolEnabled,
           Settings.Behavior.bCollisionEnabled,
           Settings.Behavior.bCastShadow}};
}

/** User Story: As a rendering distance lod consumer, I need to invoke distance within stage through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn bool DistanceWithinStage(float Distance, const FLevelDistanceLodStage &Stage) */
bool DistanceWithinStage(float Distance,
                         const FLevelDistanceLodStage &Stage) {
  return Distance <= Stage.Selection.MaxDistance;
}

} // namespace

/** User Story: As a rendering distance lod consumer, I need to invoke reduce distance lod stages through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn TArray<FLevelDistanceLodStage> ReduceDistanceLodStages( const ForbocAI::Game::Data::FLodSettings &Settings) */
TArray<FLevelDistanceLodStage> ReduceDistanceLodStages(
    const ForbocAI::Game::Data::FLodSettings &Settings) {
  return func::map_array<
      ForbocAI::Game::Data::FStageSettings,
      FLevelDistanceLodStage>(Settings.Stages, ReduceDistanceLodStageSettings);
}

/** User Story: As a rendering distance lod consumer, I need to invoke reduce distance lod stage through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn FLevelDistanceLodStage ReduceDistanceLodStage(const FLevelDistanceLodStageRequest &Request) */
FLevelDistanceLodStage
ReduceDistanceLodStage(const FLevelDistanceLodStageRequest &Request) {
  check(!Request.Stages.IsEmpty());
  const float Distance = FVector::Dist2D(Request.Origin, Request.Location);
  const func::Maybe<FLevelDistanceLodStage> Stage =
      func::find_array<FLevelDistanceLodStage>(
          Request.Stages, [Distance](const FLevelDistanceLodStage &Candidate) {
            return DistanceWithinStage(Distance, Candidate);
          });
  check(Stage.hasValue);
  return Stage.value;
}

} // namespace RenderingDistanceLodReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
