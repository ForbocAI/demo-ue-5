#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FStageSelection {
  FString Id;
  float MaxDistance;
};

struct FStageMesh {
  int32 StaticMeshForcedLodModel;
  int32 SkeletalMeshForcedLodModel;
  int32 SkeletalMeshMinLodModel;
};

struct FStageTiming {
  float CullDistance;
  float ActorTickIntervalSeconds;
};

struct FStageVisibility {
  bool bStaticVisible;
  bool bDynamicVisible;
  bool bLabelsVisible;
};

struct FStageBehavior {
  bool bAnimated;
  bool bUpdateRateOptimizationsEnabled;
  bool bPatrolEnabled;
  bool bCollisionEnabled;
  bool bCastShadow;
};

struct FLevelDistanceLodStage {
  FStageSelection Selection;
  FStageMesh Mesh;
  FStageTiming Timing;
  FStageVisibility Visibility;
  FStageBehavior Behavior;
};

struct FLevelDistanceLodStageRequest {
  FVector Origin;
  FVector Location;
  TArray<FLevelDistanceLodStage> Stages;
};

inline bool operator==(const FStageSelection &Left,
                       const FStageSelection &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance);
}
inline bool operator!=(const FStageSelection &Left,
                       const FStageSelection &Right) { return !(Left == Right); }

inline bool operator==(const FStageMesh &Left, const FStageMesh &Right) {
  return Left.StaticMeshForcedLodModel == Right.StaticMeshForcedLodModel &&
         Left.SkeletalMeshForcedLodModel == Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel;
}
inline bool operator!=(const FStageMesh &Left,
                       const FStageMesh &Right) { return !(Left == Right); }

inline bool operator==(const FStageTiming &Left,
                       const FStageTiming &Right) {
  return FMath::IsNearlyEqual(Left.CullDistance, Right.CullDistance) &&
         FMath::IsNearlyEqual(Left.ActorTickIntervalSeconds,
                              Right.ActorTickIntervalSeconds);
}
inline bool operator!=(const FStageTiming &Left,
                       const FStageTiming &Right) { return !(Left == Right); }

inline bool operator==(const FStageVisibility &Left,
                       const FStageVisibility &Right) {
  return Left.bStaticVisible == Right.bStaticVisible &&
         Left.bDynamicVisible == Right.bDynamicVisible &&
         Left.bLabelsVisible == Right.bLabelsVisible;
}
inline bool operator!=(const FStageVisibility &Left,
                       const FStageVisibility &Right) { return !(Left == Right); }

inline bool operator==(const FStageBehavior &Left,
                       const FStageBehavior &Right) {
  return Left.bAnimated == Right.bAnimated &&
         Left.bUpdateRateOptimizationsEnabled ==
             Right.bUpdateRateOptimizationsEnabled &&
         Left.bPatrolEnabled == Right.bPatrolEnabled &&
         Left.bCollisionEnabled == Right.bCollisionEnabled &&
         Left.bCastShadow == Right.bCastShadow;
}
inline bool operator!=(const FStageBehavior &Left,
                       const FStageBehavior &Right) { return !(Left == Right); }

inline bool operator==(const FLevelDistanceLodStage &Left,
                       const FLevelDistanceLodStage &Right) {
  return Left.Selection == Right.Selection && Left.Mesh == Right.Mesh &&
         Left.Timing == Right.Timing &&
         Left.Visibility == Right.Visibility && Left.Behavior == Right.Behavior;
}
inline bool operator!=(const FLevelDistanceLodStage &Left,
                       const FLevelDistanceLodStage &Right) { return !(Left == Right); }

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
