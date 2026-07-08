#include "Features/Systems/Rendering/Distance/Lod/Slice.h"

#include "Core/ue_fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingDistanceLodReducers {
namespace {

FLevelDistanceLodStage ReduceDistanceLodStageSettings(
    const ForbocAI::Game::Data::FRenderingDistanceLodStageSettings &Settings) {
  return {Settings.Id,
          Settings.MaxDistance,
          Settings.StaticMeshForcedLodModel,
          Settings.SkeletalMeshForcedLodModel,
          Settings.SkeletalMeshMinLodModel,
          Settings.CullDistance,
          Settings.ActorTickIntervalSeconds,
          Settings.bStaticVisible,
          Settings.bDynamicVisible,
          Settings.bLabelsVisible,
          Settings.bAnimated,
          Settings.bUpdateRateOptimizationsEnabled,
          Settings.bPatrolEnabled,
          Settings.bCollisionEnabled,
          Settings.bCastShadow};
}

bool DistanceWithinStage(float Distance,
                         const FLevelDistanceLodStage &Stage) {
  return Distance <= Stage.MaxDistance;
}

} // namespace

TArray<FLevelDistanceLodStage> ReduceDistanceLodStages(
    const ForbocAI::Game::Data::FRenderingDistanceLodSettings &Settings) {
  return func::map_array<
      ForbocAI::Game::Data::FRenderingDistanceLodStageSettings,
      FLevelDistanceLodStage>(Settings.Stages, ReduceDistanceLodStageSettings);
}

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
