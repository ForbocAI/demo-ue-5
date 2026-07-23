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

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageSelection &Left, const FStageSelection &Right) */
inline bool operator==(const FStageSelection &Left,
                       const FStageSelection &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance);
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageSelection &Left, const FStageSelection &Right) */
inline bool operator!=(const FStageSelection &Left,
                       const FStageSelection &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageMesh &Left, const FStageMesh &Right) */
inline bool operator==(const FStageMesh &Left, const FStageMesh &Right) {
  return Left.StaticMeshForcedLodModel == Right.StaticMeshForcedLodModel &&
         Left.SkeletalMeshForcedLodModel == Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageMesh &Left, const FStageMesh &Right) */
inline bool operator!=(const FStageMesh &Left,
                       const FStageMesh &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageTiming &Left, const FStageTiming &Right) */
inline bool operator==(const FStageTiming &Left,
                       const FStageTiming &Right) {
  return FMath::IsNearlyEqual(Left.CullDistance, Right.CullDistance) &&
         FMath::IsNearlyEqual(Left.ActorTickIntervalSeconds,
                              Right.ActorTickIntervalSeconds);
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageTiming &Left, const FStageTiming &Right) */
inline bool operator!=(const FStageTiming &Left,
                       const FStageTiming &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageVisibility &Left, const FStageVisibility &Right) */
inline bool operator==(const FStageVisibility &Left,
                       const FStageVisibility &Right) {
  return Left.bStaticVisible == Right.bStaticVisible &&
         Left.bDynamicVisible == Right.bDynamicVisible &&
         Left.bLabelsVisible == Right.bLabelsVisible;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageVisibility &Left, const FStageVisibility &Right) */
inline bool operator!=(const FStageVisibility &Left,
                       const FStageVisibility &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageBehavior &Left, const FStageBehavior &Right) */
inline bool operator==(const FStageBehavior &Left,
                       const FStageBehavior &Right) {
  return Left.bAnimated == Right.bAnimated &&
         Left.bUpdateRateOptimizationsEnabled ==
             Right.bUpdateRateOptimizationsEnabled &&
         Left.bPatrolEnabled == Right.bPatrolEnabled &&
         Left.bCollisionEnabled == Right.bCollisionEnabled &&
         Left.bCastShadow == Right.bCastShadow;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageBehavior &Left, const FStageBehavior &Right) */
inline bool operator!=(const FStageBehavior &Left,
                       const FStageBehavior &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FLevelDistanceLodStage &Left, const FLevelDistanceLodStage &Right) */
inline bool operator==(const FLevelDistanceLodStage &Left,
                       const FLevelDistanceLodStage &Right) {
  return Left.Selection == Right.Selection && Left.Mesh == Right.Mesh &&
         Left.Timing == Right.Timing &&
         Left.Visibility == Right.Visibility && Left.Behavior == Right.Behavior;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FLevelDistanceLodStage &Left, const FLevelDistanceLodStage &Right) */
inline bool operator!=(const FLevelDistanceLodStage &Left,
                       const FLevelDistanceLodStage &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FLevelDistanceLodStageRequest &Left, const FLevelDistanceLodStageRequest &Right) */
inline bool operator==(const FLevelDistanceLodStageRequest &Left,
                       const FLevelDistanceLodStageRequest &Right) {
  return Left.Origin == Right.Origin && Left.Location == Right.Location &&
         Left.Stages == Right.Stages;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FLevelDistanceLodStageRequest &Left, const FLevelDistanceLodStageRequest &Right) */
inline bool operator!=(const FLevelDistanceLodStageRequest &Left,
                       const FLevelDistanceLodStageRequest &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
