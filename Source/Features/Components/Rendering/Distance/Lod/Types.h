#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FLevelDistanceLodStage {
  FString Id;
  float MaxDistance;
  int32 StaticMeshForcedLodModel;
  int32 SkeletalMeshForcedLodModel;
  int32 SkeletalMeshMinLodModel;
  float CullDistance;
  float ActorTickIntervalSeconds;
  bool bStaticVisible;
  bool bDynamicVisible;
  bool bLabelsVisible;
  bool bAnimated;
  bool bUpdateRateOptimizationsEnabled;
  bool bPatrolEnabled;
  bool bCollisionEnabled;
  bool bCastShadow;
};

struct FLevelDistanceLodStageRequest {
  FVector Origin;
  FVector Location;
  TArray<FLevelDistanceLodStage> Stages;
};

inline bool operator==(const FLevelDistanceLodStage &Left,
                       const FLevelDistanceLodStage &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance) &&
         Left.StaticMeshForcedLodModel == Right.StaticMeshForcedLodModel &&
         Left.SkeletalMeshForcedLodModel == Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel &&
         FMath::IsNearlyEqual(Left.CullDistance, Right.CullDistance) &&
         FMath::IsNearlyEqual(Left.ActorTickIntervalSeconds,
                              Right.ActorTickIntervalSeconds) &&
         Left.bStaticVisible == Right.bStaticVisible &&
         Left.bDynamicVisible == Right.bDynamicVisible &&
         Left.bLabelsVisible == Right.bLabelsVisible &&
         Left.bAnimated == Right.bAnimated &&
         Left.bUpdateRateOptimizationsEnabled ==
             Right.bUpdateRateOptimizationsEnabled &&
         Left.bPatrolEnabled == Right.bPatrolEnabled &&
         Left.bCollisionEnabled == Right.bCollisionEnabled &&
         Left.bCastShadow == Right.bCastShadow;
}

inline bool operator!=(const FLevelDistanceLodStage &Left,
                       const FLevelDistanceLodStage &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelDistanceLodStageRequest &Left,
                       const FLevelDistanceLodStageRequest &Right) {
  return Left.Origin == Right.Origin && Left.Location == Right.Location &&
         Left.Stages == Right.Stages;
}

inline bool operator!=(const FLevelDistanceLodStageRequest &Left,
                       const FLevelDistanceLodStageRequest &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
