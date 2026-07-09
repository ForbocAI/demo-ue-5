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

inline bool operator==(const FStageSelectionSettings &Left,
                       const FStageSelectionSettings &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance);
}
inline bool operator!=(const FStageSelectionSettings &Left,
                       const FStageSelectionSettings &Right) { return !(Left == Right); }

inline bool operator==(const FStageMeshSettings &Left,
                       const FStageMeshSettings &Right) {
  return Left.StaticMeshForcedLodModel == Right.StaticMeshForcedLodModel &&
         Left.SkeletalMeshForcedLodModel == Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel;
}
inline bool operator!=(const FStageMeshSettings &Left,
                       const FStageMeshSettings &Right) { return !(Left == Right); }

inline bool operator==(const FStageTimingSettings &Left,
                       const FStageTimingSettings &Right) {
  return FMath::IsNearlyEqual(Left.CullDistance, Right.CullDistance) &&
         FMath::IsNearlyEqual(Left.ActorTickIntervalSeconds,
                              Right.ActorTickIntervalSeconds);
}
inline bool operator!=(const FStageTimingSettings &Left,
                       const FStageTimingSettings &Right) { return !(Left == Right); }

inline bool operator==(const FStageVisibilitySettings &Left,
                       const FStageVisibilitySettings &Right) {
  return Left.bStaticVisible == Right.bStaticVisible &&
         Left.bDynamicVisible == Right.bDynamicVisible &&
         Left.bLabelsVisible == Right.bLabelsVisible;
}
inline bool operator!=(const FStageVisibilitySettings &Left,
                       const FStageVisibilitySettings &Right) { return !(Left == Right); }

inline bool operator==(const FStageBehaviorSettings &Left,
                       const FStageBehaviorSettings &Right) {
  return Left.bAnimated == Right.bAnimated &&
         Left.bUpdateRateOptimizationsEnabled ==
             Right.bUpdateRateOptimizationsEnabled &&
         Left.bPatrolEnabled == Right.bPatrolEnabled &&
         Left.bCollisionEnabled == Right.bCollisionEnabled &&
         Left.bCastShadow == Right.bCastShadow;
}
inline bool operator!=(const FStageBehaviorSettings &Left,
                       const FStageBehaviorSettings &Right) { return !(Left == Right); }

inline bool operator==(const FStageSettings &Left,
                       const FStageSettings &Right) {
  return Left.Selection == Right.Selection && Left.Mesh == Right.Mesh &&
         Left.Timing == Right.Timing &&
         Left.Visibility == Right.Visibility && Left.Behavior == Right.Behavior;
}
inline bool operator!=(const FStageSettings &Left,
                       const FStageSettings &Right) { return !(Left == Right); }

inline bool operator==(const FLodSettings &Left,
                       const FLodSettings &Right) {
  return Left.Stages == Right.Stages;
}
inline bool operator!=(const FLodSettings &Left,
                       const FLodSettings &Right) { return !(Left == Right); }

} // namespace Data
} // namespace Game
} // namespace ForbocAI
