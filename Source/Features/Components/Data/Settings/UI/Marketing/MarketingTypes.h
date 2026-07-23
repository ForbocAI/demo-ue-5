#pragma once

#include "Core/rtk.hpp"
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
  FString CaptureCommandLineKey;
  FString QuitWhenDoneCommandLineKey;
  FString OutputDirectoryCommandLineKey;
  FString InitialDelayCommandLineKey;
  FString SettleSecondsCommandLineKey;
  FString BetweenSecondsCommandLineKey;
  FString DefaultOutputDirectory;
  FString ConsoleQuitCommand;
  FString ScreenshotLogFormat;
  float InitialDelaySeconds;
  float SettleSeconds;
  float BetweenSeconds;
  float MenuViewportWidth;
  float MenuViewportHeight;
  float MenuAnchorX;
  float MenuAnchorY;
  float MenuAlignmentX;
  float MenuAlignmentY;
  float MenuPositionX;
  float MenuPositionY;
  int32 MenuZOrder;
  FString MenuTitle;
  FString MenuRetakeButtonText;
  FString MenuResumeButtonText;
  float MenuPanelPadding;
  float MenuTitleSize;
  float MenuButtonTextSize;
  FLinearColor MenuPanelColor;
  FLinearColor MenuTitleColor;
  FLinearColor MenuButtonTextColor;
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
  return Left.CaptureCommandLineKey == Right.CaptureCommandLineKey &&
         Left.QuitWhenDoneCommandLineKey ==
             Right.QuitWhenDoneCommandLineKey &&
         Left.OutputDirectoryCommandLineKey ==
             Right.OutputDirectoryCommandLineKey &&
         Left.InitialDelayCommandLineKey ==
             Right.InitialDelayCommandLineKey &&
         Left.SettleSecondsCommandLineKey ==
             Right.SettleSecondsCommandLineKey &&
         Left.BetweenSecondsCommandLineKey ==
             Right.BetweenSecondsCommandLineKey &&
         Left.DefaultOutputDirectory == Right.DefaultOutputDirectory &&
         Left.ConsoleQuitCommand == Right.ConsoleQuitCommand &&
         Left.ScreenshotLogFormat == Right.ScreenshotLogFormat &&
         FMath::IsNearlyEqual(Left.InitialDelaySeconds,
                              Right.InitialDelaySeconds) &&
         FMath::IsNearlyEqual(Left.SettleSeconds, Right.SettleSeconds) &&
         FMath::IsNearlyEqual(Left.BetweenSeconds, Right.BetweenSeconds) &&
         FMath::IsNearlyEqual(Left.MenuViewportWidth,
                              Right.MenuViewportWidth) &&
         FMath::IsNearlyEqual(Left.MenuViewportHeight,
                              Right.MenuViewportHeight) &&
         FMath::IsNearlyEqual(Left.MenuAnchorX, Right.MenuAnchorX) &&
         FMath::IsNearlyEqual(Left.MenuAnchorY, Right.MenuAnchorY) &&
         FMath::IsNearlyEqual(Left.MenuAlignmentX, Right.MenuAlignmentX) &&
         FMath::IsNearlyEqual(Left.MenuAlignmentY, Right.MenuAlignmentY) &&
         FMath::IsNearlyEqual(Left.MenuPositionX, Right.MenuPositionX) &&
         FMath::IsNearlyEqual(Left.MenuPositionY, Right.MenuPositionY) &&
         Left.MenuZOrder == Right.MenuZOrder &&
         Left.MenuTitle == Right.MenuTitle &&
         Left.MenuRetakeButtonText == Right.MenuRetakeButtonText &&
         Left.MenuResumeButtonText == Right.MenuResumeButtonText &&
         FMath::IsNearlyEqual(Left.MenuPanelPadding,
                              Right.MenuPanelPadding) &&
         FMath::IsNearlyEqual(Left.MenuTitleSize, Right.MenuTitleSize) &&
         FMath::IsNearlyEqual(Left.MenuButtonTextSize,
                              Right.MenuButtonTextSize) &&
         Left.MenuPanelColor == Right.MenuPanelColor &&
         Left.MenuTitleColor == Right.MenuTitleColor &&
         Left.MenuButtonTextColor == Right.MenuButtonTextColor &&
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
