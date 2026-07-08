#include "Features/Systems/Capture/Adapters.h"

#include "Features/Components/Data/Settings/UI/Types.h"
#include "HAL/FileManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "UnrealClient.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace CaptureAdapters {

float CommandLineFloat(const FString &Key, float Fallback) {
  float Value = Fallback;
  FParse::Value(FCommandLine::Get(), *Key, Value);
  return Value;
}

FString CommandLineString(const FString &Key, const FString &Fallback) {
  FString Value = Fallback;
  FParse::Value(FCommandLine::Get(), *Key, Value);
  return Value;
}

bool CommandLineParam(const FString &Key) {
  return FParse::Param(FCommandLine::Get(), *Key);
}

FString ResolveCaptureOutputPath(const FString &RelativeDir) {
  return FPaths::ConvertRelativePathToFull(
      FPaths::Combine(FPaths::ProjectDir(), RelativeDir));
}

void CreateCaptureOutputDirectory(const FString &Directory) {
  IFileManager::Get().MakeDirectory(*Directory, true);
}

void RequestCaptureScreenshot(const FString &OutputDir,
                              const FString &OutputName) {
  const FString OutputPath = FPaths::Combine(OutputDir, OutputName);
  FScreenshotRequest::RequestScreenshot(OutputPath, false, false);
}

FScaleAuditCaptureConfig ParseScaleAuditCommandLine(
    const ForbocAI::Game::Data::FScaleAuditCaptureSettings &Settings,
    float TerrainWorldSize) {
  const FString DefaultOutputDirectory =
      ResolveCaptureOutputPath(Settings.DefaultOutputDirectory);
  FScaleAuditCaptureConfig Config;
  Config.bQuitWhenDone = CommandLineParam(Settings.QuitWhenDoneCommandLineKey);
  Config.OutputDirectory = CommandLineString(
      Settings.OutputDirectoryCommandLineKey, DefaultOutputDirectory);
  Config.InitialDelaySeconds = CommandLineFloat(
      Settings.InitialDelayCommandLineKey, Settings.InitialDelaySeconds);
  Config.SettleSeconds = CommandLineFloat(
      Settings.SettleSecondsCommandLineKey, Settings.SettleSeconds);
  Config.BetweenSeconds = CommandLineFloat(
      Settings.BetweenSecondsCommandLineKey, Settings.BetweenSeconds);
  Config.WholeOrthoWidth = CommandLineFloat(
      Settings.WholeOrthoWidthCommandLineKey, TerrainWorldSize);
  Config.TownOrthoWidth = CommandLineFloat(
      Settings.TownOrthoWidthCommandLineKey, TerrainWorldSize);
  Config.ActorsOrthoWidth = CommandLineFloat(
      Settings.ActorsOrthoWidthCommandLineKey, TerrainWorldSize);
  Config.WholeCaptureHeight = CommandLineFloat(
      Settings.WholeCaptureHeightCommandLineKey, TerrainWorldSize);
  Config.TownCaptureHeight = CommandLineFloat(
      Settings.TownCaptureHeightCommandLineKey, Config.TownOrthoWidth);
  Config.ActorsCaptureHeight = CommandLineFloat(
      Settings.ActorsCaptureHeightCommandLineKey, Config.ActorsOrthoWidth);
  CreateCaptureOutputDirectory(Config.OutputDirectory);
  return Config;
}

FMarketingCaptureConfig ParseMarketingCommandLine(
    const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings) {
  const FString DefaultOutputDirectory =
      ResolveCaptureOutputPath(Settings.DefaultOutputDirectory);
  FMarketingCaptureConfig Config;
  Config.bQuitWhenDone = CommandLineParam(Settings.QuitWhenDoneCommandLineKey);
  Config.OutputDirectory = CommandLineString(
      Settings.OutputDirectoryCommandLineKey, DefaultOutputDirectory);
  Config.InitialDelaySeconds = CommandLineFloat(
      Settings.InitialDelayCommandLineKey, Settings.InitialDelaySeconds);
  Config.SettleSeconds = CommandLineFloat(
      Settings.SettleSecondsCommandLineKey, Settings.SettleSeconds);
  Config.BetweenSeconds = CommandLineFloat(
      Settings.BetweenSecondsCommandLineKey, Settings.BetweenSeconds);
  CreateCaptureOutputDirectory(Config.OutputDirectory);
  return Config;
}

} // namespace CaptureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
