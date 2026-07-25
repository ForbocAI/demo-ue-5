#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRegionCapture {
  FString OrthoWidthCommandLineKey;
  FString CaptureHeightCommandLineKey;
  FString OutputName;
};

struct FTiming {
  FString InitialDelayCommandLineKey;
  FString SettleSecondsCommandLineKey;
  FString BetweenSecondsCommandLineKey;
  float InitialDelaySeconds;
  float SettleSeconds;
  float BetweenSeconds;
};

struct FAuditCaptureSettings {
  FString CaptureCommandLineKey;
  FString QuitWhenDoneCommandLineKey;
  FString OutputDirectoryCommandLineKey;
  FString DefaultOutputDirectory;
  FTiming Timing;
  FRegionCapture Whole;
  FRegionCapture Town;
  FRegionCapture Actors;
  FRotator TopDownRotation;
};

/** User Story: As a ui scale audit consumer, I need to compare values for equality through a stable signature so the ui scale audit workflow remains explicit and composable. @fn inline bool operator==(const FAuditCaptureSettings &Left, const FAuditCaptureSettings &Right) */
inline bool operator==(const FRegionCapture &Left,
                       const FRegionCapture &Right) {
  return Left.OrthoWidthCommandLineKey == Right.OrthoWidthCommandLineKey &&
         Left.CaptureHeightCommandLineKey ==
             Right.CaptureHeightCommandLineKey &&
         Left.OutputName == Right.OutputName;
}

/** User Story: As a ui scale audit consumer, I need to compare values for inequality through a stable signature so the ui scale audit workflow remains explicit and composable. @fn inline bool operator!=(const FRegionCapture &Left, const FRegionCapture &Right) */
inline bool operator!=(const FRegionCapture &Left,
                       const FRegionCapture &Right) {
  return !(Left == Right);
}

/** User Story: As a ui scale audit consumer, I need to compare values for equality through a stable signature so the ui scale audit workflow remains explicit and composable. @fn inline bool operator==(const FTiming &Left, const FTiming &Right) */
inline bool operator==(const FTiming &Left, const FTiming &Right) {
  return Left.InitialDelayCommandLineKey ==
             Right.InitialDelayCommandLineKey &&
         Left.SettleSecondsCommandLineKey ==
             Right.SettleSecondsCommandLineKey &&
         Left.BetweenSecondsCommandLineKey ==
             Right.BetweenSecondsCommandLineKey &&
         FMath::IsNearlyEqual(Left.InitialDelaySeconds,
                              Right.InitialDelaySeconds) &&
         FMath::IsNearlyEqual(Left.SettleSeconds, Right.SettleSeconds) &&
         FMath::IsNearlyEqual(Left.BetweenSeconds, Right.BetweenSeconds);
}

/** User Story: As a ui scale audit consumer, I need to compare values for inequality through a stable signature so the ui scale audit workflow remains explicit and composable. @fn inline bool operator!=(const FTiming &Left, const FTiming &Right) */
inline bool operator!=(const FTiming &Left, const FTiming &Right) {
  return !(Left == Right);
}

/** User Story: As a ui scale audit consumer, I need to compare values for equality through a stable signature so the ui scale audit workflow remains explicit and composable. @fn inline bool operator==(const FAuditCaptureSettings &Left, const FAuditCaptureSettings &Right) */
inline bool operator==(const FAuditCaptureSettings &Left,
                       const FAuditCaptureSettings &Right) {
  return Left.CaptureCommandLineKey == Right.CaptureCommandLineKey &&
         Left.QuitWhenDoneCommandLineKey ==
             Right.QuitWhenDoneCommandLineKey &&
         Left.OutputDirectoryCommandLineKey ==
             Right.OutputDirectoryCommandLineKey &&
         Left.DefaultOutputDirectory == Right.DefaultOutputDirectory &&
         Left.Timing == Right.Timing && Left.Whole == Right.Whole &&
         Left.Town == Right.Town && Left.Actors == Right.Actors &&
         Left.TopDownRotation.Equals(Right.TopDownRotation);
}

/** User Story: As a ui scale audit consumer, I need to compare values for inequality through a stable signature so the ui scale audit workflow remains explicit and composable. @fn inline bool operator!=(const FAuditCaptureSettings &Left, const FAuditCaptureSettings &Right) */
inline bool operator!=(const FAuditCaptureSettings &Left,
                       const FAuditCaptureSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
