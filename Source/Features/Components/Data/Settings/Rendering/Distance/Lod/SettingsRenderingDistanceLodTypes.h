#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FStageSelectionSettings {
  FString Id;
  float MaxDistance;
};

struct FStageMeshSettings {
  int32 StaticMeshForcedLodModel;
  int32 SkeletalMeshForcedLodModel;
  int32 SkeletalMeshMinLodModel;
};

struct FStageTimingSettings {
  float CullDistance;
  float ActorTickIntervalSeconds;
};

struct FStageVisibilitySettings {
  bool bStaticVisible;
  bool bDynamicVisible;
  bool bLabelsVisible;
};

struct FStageBehaviorSettings {
  bool bAnimated;
  bool bUpdateRateOptimizationsEnabled;
  bool bPatrolEnabled;
  bool bCollisionEnabled;
  bool bCastShadow;
};

struct FStageSettings {
  FStageSelectionSettings Selection;
  FStageMeshSettings Mesh;
  FStageTimingSettings Timing;
  FStageVisibilitySettings Visibility;
  FStageBehaviorSettings Behavior;
};

struct FLodSettings {
  TArray<FStageSettings> Stages;
};

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageSelectionSettings &Left, const FStageSelectionSettings &Right) */
inline bool operator==(const FStageSelectionSettings &Left,
                       const FStageSelectionSettings &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance);
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageSelectionSettings &Left, const FStageSelectionSettings &Right) */
inline bool operator!=(const FStageSelectionSettings &Left,
                       const FStageSelectionSettings &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageMeshSettings &Left, const FStageMeshSettings &Right) */
inline bool operator==(const FStageMeshSettings &Left,
                       const FStageMeshSettings &Right) {
  return Left.StaticMeshForcedLodModel == Right.StaticMeshForcedLodModel &&
         Left.SkeletalMeshForcedLodModel == Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageMeshSettings &Left, const FStageMeshSettings &Right) */
inline bool operator!=(const FStageMeshSettings &Left,
                       const FStageMeshSettings &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageTimingSettings &Left, const FStageTimingSettings &Right) */
inline bool operator==(const FStageTimingSettings &Left,
                       const FStageTimingSettings &Right) {
  return FMath::IsNearlyEqual(Left.CullDistance, Right.CullDistance) &&
         FMath::IsNearlyEqual(Left.ActorTickIntervalSeconds,
                              Right.ActorTickIntervalSeconds);
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageTimingSettings &Left, const FStageTimingSettings &Right) */
inline bool operator!=(const FStageTimingSettings &Left,
                       const FStageTimingSettings &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageVisibilitySettings &Left, const FStageVisibilitySettings &Right) */
inline bool operator==(const FStageVisibilitySettings &Left,
                       const FStageVisibilitySettings &Right) {
  return Left.bStaticVisible == Right.bStaticVisible &&
         Left.bDynamicVisible == Right.bDynamicVisible &&
         Left.bLabelsVisible == Right.bLabelsVisible;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageVisibilitySettings &Left, const FStageVisibilitySettings &Right) */
inline bool operator!=(const FStageVisibilitySettings &Left,
                       const FStageVisibilitySettings &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageBehaviorSettings &Left, const FStageBehaviorSettings &Right) */
inline bool operator==(const FStageBehaviorSettings &Left,
                       const FStageBehaviorSettings &Right) {
  return Left.bAnimated == Right.bAnimated &&
         Left.bUpdateRateOptimizationsEnabled ==
             Right.bUpdateRateOptimizationsEnabled &&
         Left.bPatrolEnabled == Right.bPatrolEnabled &&
         Left.bCollisionEnabled == Right.bCollisionEnabled &&
         Left.bCastShadow == Right.bCastShadow;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageBehaviorSettings &Left, const FStageBehaviorSettings &Right) */
inline bool operator!=(const FStageBehaviorSettings &Left,
                       const FStageBehaviorSettings &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FStageSettings &Left, const FStageSettings &Right) */
inline bool operator==(const FStageSettings &Left,
                       const FStageSettings &Right) {
  return Left.Selection == Right.Selection && Left.Mesh == Right.Mesh &&
         Left.Timing == Right.Timing &&
         Left.Visibility == Right.Visibility && Left.Behavior == Right.Behavior;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FStageSettings &Left, const FStageSettings &Right) */
inline bool operator!=(const FStageSettings &Left,
                       const FStageSettings &Right) { return !(Left == Right); }

/** User Story: As a rendering distance lod consumer, I need to compare values for equality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator==(const FLodSettings &Left, const FLodSettings &Right) */
inline bool operator==(const FLodSettings &Left,
                       const FLodSettings &Right) {
  return Left.Stages == Right.Stages;
}
/** User Story: As a rendering distance lod consumer, I need to compare values for inequality through a stable signature so the rendering distance lod workflow remains explicit and composable. @fn inline bool operator!=(const FLodSettings &Left, const FLodSettings &Right) */
inline bool operator!=(const FLodSettings &Left,
                       const FLodSettings &Right) { return !(Left == Right); }

} // namespace Data
} // namespace Game
} // namespace ForbocAI
