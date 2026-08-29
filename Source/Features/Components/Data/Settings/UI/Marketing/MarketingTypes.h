#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/UI/Capture/Timing/TimingTypes.h"
#include "Features/Components/Data/Settings/UI/Marketing/Command/CommandTypes.h"
#include "Features/Components/Data/Settings/UI/Marketing/Menu/Colors/MenuColorTypes.h"
#include "Features/Components/Data/Settings/UI/Marketing/Menu/Content/ContentTypes.h"
#include "Features/Components/Data/Settings/UI/Marketing/Menu/Placement/PlacementTypes.h"
#include "Features/Components/Data/Settings/UI/Marketing/Menu/Typography/TypographyTypes.h"
#include "Features/Components/Data/Settings/UI/Marketing/Menu/Viewport/ViewportTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCaptureOutputSettings {
  FString OutputName;
};

struct FCaptureCameraSettings {
  FVector CameraOffset;
  FVector TargetOffset;
  float OrthoWidth;
  float FieldOfView;
  float SpringArmLength;
};

struct FCaptureVisibilitySettings {
  bool bHidePlayerMesh;
  bool bUseActorRouteCenter;
};

struct FCaptureViewSettings {
  FCaptureOutputSettings Output;
  FCaptureCameraSettings Camera;
  FCaptureVisibilitySettings Visibility;
};

struct FMarketingCaptureSettings {
  FCommandSettings Command;
  FTiming Timing;
  FViewportSettings Viewport;
  FPlacementSettings Placement;
  FContentSettings Content;
  FTypographySettings Typography;
  FMenuColorSettings Colors;
  TArray<FCaptureViewSettings> CaptureViews;
};

/** User Story: As a settings ui marketing consumer, I need to compare values for equality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator==(const FCaptureOutputSettings &Left, const FCaptureOutputSettings &Right) */
inline bool operator==(const FCaptureOutputSettings &Left,
                       const FCaptureOutputSettings &Right) {
  return Left.OutputName == Right.OutputName;
}

/** User Story: As a settings ui marketing consumer, I need to compare values for inequality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator!=(const FCaptureOutputSettings &Left, const FCaptureOutputSettings &Right) */
inline bool operator!=(const FCaptureOutputSettings &Left,
                       const FCaptureOutputSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a settings ui marketing consumer, I need to compare values for equality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator==(const FCaptureCameraSettings &Left, const FCaptureCameraSettings &Right) */
inline bool operator==(const FCaptureCameraSettings &Left,
                       const FCaptureCameraSettings &Right) {
  return Left.CameraOffset == Right.CameraOffset &&
         Left.TargetOffset == Right.TargetOffset &&
         FMath::IsNearlyEqual(Left.OrthoWidth, Right.OrthoWidth) &&
         FMath::IsNearlyEqual(Left.FieldOfView, Right.FieldOfView) &&
         FMath::IsNearlyEqual(Left.SpringArmLength, Right.SpringArmLength);
}

/** User Story: As a settings ui marketing consumer, I need to compare values for inequality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator!=(const FCaptureCameraSettings &Left, const FCaptureCameraSettings &Right) */
inline bool operator!=(const FCaptureCameraSettings &Left,
                       const FCaptureCameraSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a settings ui marketing consumer, I need to compare values for equality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator==(const FCaptureVisibilitySettings &Left, const FCaptureVisibilitySettings &Right) */
inline bool operator==(const FCaptureVisibilitySettings &Left,
                       const FCaptureVisibilitySettings &Right) {
  return Left.bHidePlayerMesh == Right.bHidePlayerMesh &&
         Left.bUseActorRouteCenter == Right.bUseActorRouteCenter;
}

/** User Story: As a settings ui marketing consumer, I need to compare values for inequality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator!=(const FCaptureVisibilitySettings &Left, const FCaptureVisibilitySettings &Right) */
inline bool operator!=(const FCaptureVisibilitySettings &Left,
                       const FCaptureVisibilitySettings &Right) {
  return !(Left == Right);
}

/** User Story: As a settings ui marketing consumer, I need to compare values for equality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator==(const FCaptureViewSettings &Left, const FCaptureViewSettings &Right) */
inline bool operator==(const FCaptureViewSettings &Left,
                       const FCaptureViewSettings &Right) {
  return Left.Output == Right.Output &&
         Left.Camera == Right.Camera &&
         Left.Visibility == Right.Visibility;
}

/** User Story: As a settings ui marketing consumer, I need to compare values for inequality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator!=(const FCaptureViewSettings &Left, const FCaptureViewSettings &Right) */
inline bool operator!=(const FCaptureViewSettings &Left,
                       const FCaptureViewSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a settings ui marketing consumer, I need to compare values for equality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator==(const FMarketingCaptureSettings &Left, const FMarketingCaptureSettings &Right) */
inline bool operator==(const FMarketingCaptureSettings &Left,
                       const FMarketingCaptureSettings &Right) {
  return Left.Command == Right.Command && Left.Timing == Right.Timing &&
         Left.Viewport == Right.Viewport &&
         Left.Placement == Right.Placement &&
         Left.Content == Right.Content &&
         Left.Typography == Right.Typography &&
         Left.Colors == Right.Colors &&
         Left.CaptureViews == Right.CaptureViews;
}

/** User Story: As a settings ui marketing consumer, I need to compare values for inequality through a stable signature so the settings ui marketing workflow remains explicit and composable. @fn inline bool operator!=(const FMarketingCaptureSettings &Left, const FMarketingCaptureSettings &Right) */
inline bool operator!=(const FMarketingCaptureSettings &Left,
                       const FMarketingCaptureSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
