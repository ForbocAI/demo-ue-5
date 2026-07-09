#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FAuditCaptureSettings {
  FString CaptureCommandLineKey;
  FString QuitWhenDoneCommandLineKey;
  FString OutputDirectoryCommandLineKey;
  FString InitialDelayCommandLineKey;
  FString SettleSecondsCommandLineKey;
  FString BetweenSecondsCommandLineKey;
  FString WholeOrthoWidthCommandLineKey;
  FString TownOrthoWidthCommandLineKey;
  FString ActorsOrthoWidthCommandLineKey;
  FString WholeCaptureHeightCommandLineKey;
  FString TownCaptureHeightCommandLineKey;
  FString ActorsCaptureHeightCommandLineKey;
  FString DefaultOutputDirectory;
  FString WholeOutputName;
  FString TownOutputName;
  FString ActorsOutputName;
  float InitialDelaySeconds;
  float SettleSeconds;
  float BetweenSeconds;
  FRotator TopDownRotation;
};

inline bool operator==(const FAuditCaptureSettings &Left,
                       const FAuditCaptureSettings &Right) {
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
         Left.WholeOrthoWidthCommandLineKey ==
             Right.WholeOrthoWidthCommandLineKey &&
         Left.TownOrthoWidthCommandLineKey ==
             Right.TownOrthoWidthCommandLineKey &&
         Left.ActorsOrthoWidthCommandLineKey ==
             Right.ActorsOrthoWidthCommandLineKey &&
         Left.WholeCaptureHeightCommandLineKey ==
             Right.WholeCaptureHeightCommandLineKey &&
         Left.TownCaptureHeightCommandLineKey ==
             Right.TownCaptureHeightCommandLineKey &&
         Left.ActorsCaptureHeightCommandLineKey ==
             Right.ActorsCaptureHeightCommandLineKey &&
         Left.DefaultOutputDirectory == Right.DefaultOutputDirectory &&
         Left.WholeOutputName == Right.WholeOutputName &&
         Left.TownOutputName == Right.TownOutputName &&
         Left.ActorsOutputName == Right.ActorsOutputName &&
         FMath::IsNearlyEqual(Left.InitialDelaySeconds,
                              Right.InitialDelaySeconds) &&
         FMath::IsNearlyEqual(Left.SettleSeconds, Right.SettleSeconds) &&
         FMath::IsNearlyEqual(Left.BetweenSeconds, Right.BetweenSeconds) &&
         Left.TopDownRotation.Equals(Right.TopDownRotation);
}

inline bool operator!=(const FAuditCaptureSettings &Left,
                       const FAuditCaptureSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
