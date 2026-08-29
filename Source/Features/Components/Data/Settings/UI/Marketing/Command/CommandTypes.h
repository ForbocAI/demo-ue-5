#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCommandSettings {
  FString CaptureCommandLineKey;
  FString QuitWhenDoneCommandLineKey;
  FString OutputDirectoryCommandLineKey;
  FString DefaultOutputDirectory;
  FString ConsoleQuitCommand;
  FString ScreenshotLogFormat;
};

/** User Story: As a marketing capture consumer, I need to compare command settings so capture invocation remains deterministic. @fn inline bool operator==(const FCommandSettings &Left, const FCommandSettings &Right) */
inline bool operator==(const FCommandSettings &Left,
                       const FCommandSettings &Right) {
  return Left.CaptureCommandLineKey == Right.CaptureCommandLineKey &&
         Left.QuitWhenDoneCommandLineKey ==
             Right.QuitWhenDoneCommandLineKey &&
         Left.OutputDirectoryCommandLineKey ==
             Right.OutputDirectoryCommandLineKey &&
         Left.DefaultOutputDirectory == Right.DefaultOutputDirectory &&
         Left.ConsoleQuitCommand == Right.ConsoleQuitCommand &&
         Left.ScreenshotLogFormat == Right.ScreenshotLogFormat;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
