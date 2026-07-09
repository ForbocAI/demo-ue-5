#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCaptureViewSettings {
  FString OutputName;
  FVector CameraOffset;
  FVector TargetOffset;
  float OrthoWidth;
  float FieldOfView;
  float SpringArmLength;
  bool bHidePlayerMesh;
  bool bUseActorRouteCenter;
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

inline bool operator==(const FCaptureViewSettings &Left,
                       const FCaptureViewSettings &Right) {
  return Left.OutputName == Right.OutputName &&
         Left.CameraOffset == Right.CameraOffset &&
         Left.TargetOffset == Right.TargetOffset &&
         FMath::IsNearlyEqual(Left.OrthoWidth, Right.OrthoWidth) &&
         FMath::IsNearlyEqual(Left.FieldOfView, Right.FieldOfView) &&
         FMath::IsNearlyEqual(Left.SpringArmLength, Right.SpringArmLength) &&
         Left.bHidePlayerMesh == Right.bHidePlayerMesh &&
         Left.bUseActorRouteCenter == Right.bUseActorRouteCenter;
}

inline bool operator!=(const FCaptureViewSettings &Left,
                       const FCaptureViewSettings &Right) {
  return !(Left == Right);
}

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

inline bool operator!=(const FMarketingCaptureSettings &Left,
                       const FMarketingCaptureSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
