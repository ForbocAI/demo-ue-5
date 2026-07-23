#include "Features/Systems/Capture/CaptureAdapters.h"

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "HAL/FileManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "UnrealClient.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace CaptureAdapters {

/** User Story: As a features systems capture consumer, I need to invoke command line float through a stable signature so the features systems capture workflow remains explicit and composable. @fn float CommandLineFloat(const FString &Key, float Fallback) */
float CommandLineFloat(const FString &Key, float Fallback) {
  float Value = Fallback;
  FParse::Value(FCommandLine::Get(), *Key, Value);
  return Value;
}

/** User Story: As a features systems capture consumer, I need to invoke command line string through a stable signature so the features systems capture workflow remains explicit and composable. @fn FString CommandLineString(const FString &Key, const FString &Fallback) */
FString CommandLineString(const FString &Key, const FString &Fallback) {
  FString Value = Fallback;
  FParse::Value(FCommandLine::Get(), *Key, Value);
  return Value;
}

/** User Story: As a features systems capture consumer, I need to invoke command line param through a stable signature so the features systems capture workflow remains explicit and composable. @fn bool CommandLineParam(const FString &Key) */
bool CommandLineParam(const FString &Key) {
  return FParse::Param(FCommandLine::Get(), *Key);
}

/** User Story: As a features systems capture consumer, I need to invoke resolve capture output path through a stable signature so the features systems capture workflow remains explicit and composable. @fn FString ResolveCaptureOutputPath(const FString &RelativeDir) */
FString ResolveCaptureOutputPath(const FString &RelativeDir) {
  return FPaths::ConvertRelativePathToFull(
      FPaths::Combine(FPaths::ProjectDir(), RelativeDir));
}

/** User Story: As a features systems capture consumer, I need to invoke create capture output directory through a stable signature so the features systems capture workflow remains explicit and composable. @fn void CreateCaptureOutputDirectory(const FString &Directory) */
void CreateCaptureOutputDirectory(const FString &Directory) {
  IFileManager::Get().MakeDirectory(*Directory, true);
}

/** User Story: As a features systems capture consumer, I need to invoke request capture screenshot through a stable signature so the features systems capture workflow remains explicit and composable. @fn void RequestCaptureScreenshot(const FString &OutputDir, const FString &OutputName) */
void RequestCaptureScreenshot(const FString &OutputDir,
                              const FString &OutputName) {
  const FString OutputPath = FPaths::Combine(OutputDir, OutputName);
  FScreenshotRequest::RequestScreenshot(OutputPath, false, false);
}

/** User Story: As a features systems capture consumer, I need to invoke parse scale audit command line through a stable signature so the features systems capture workflow remains explicit and composable. @fn FScaleAuditCaptureConfig ParseScaleAuditCommandLine( const ForbocAI::Game::Data::FAuditCaptureSettings &Settings, float TerrainWorldSize) */
FScaleAuditCaptureConfig ParseScaleAuditCommandLine(
    const ForbocAI::Game::Data::FAuditCaptureSettings &Settings,
    float TerrainWorldSize) {
  const FString DefaultOutputDirectory =
      ResolveCaptureOutputPath(Settings.DefaultOutputDirectory);
  FScaleAuditCaptureConfig Config;
  Config.Run.bQuitWhenDone =
      CommandLineParam(Settings.QuitWhenDoneCommandLineKey);
  Config.Run.OutputDirectory = CommandLineString(
      Settings.OutputDirectoryCommandLineKey, DefaultOutputDirectory);
  Config.Timing.InitialDelaySeconds = CommandLineFloat(
      Settings.InitialDelayCommandLineKey, Settings.InitialDelaySeconds);
  Config.Timing.SettleSeconds = CommandLineFloat(
      Settings.SettleSecondsCommandLineKey, Settings.SettleSeconds);
  Config.Timing.BetweenSeconds = CommandLineFloat(
      Settings.BetweenSecondsCommandLineKey, Settings.BetweenSeconds);
  Config.Ortho.WholeWidth = CommandLineFloat(
      Settings.WholeOrthoWidthCommandLineKey, TerrainWorldSize);
  Config.Ortho.TownWidth = CommandLineFloat(
      Settings.TownOrthoWidthCommandLineKey, TerrainWorldSize);
  Config.Ortho.ActorsWidth = CommandLineFloat(
      Settings.ActorsOrthoWidthCommandLineKey, TerrainWorldSize);
  Config.Height.WholeHeight = CommandLineFloat(
      Settings.WholeCaptureHeightCommandLineKey, TerrainWorldSize);
  Config.Height.TownHeight = CommandLineFloat(
      Settings.TownCaptureHeightCommandLineKey, Config.Ortho.TownWidth);
  Config.Height.ActorsHeight = CommandLineFloat(
      Settings.ActorsCaptureHeightCommandLineKey, Config.Ortho.ActorsWidth);
  CreateCaptureOutputDirectory(Config.Run.OutputDirectory);
  return Config;
}

/** User Story: As a features systems capture consumer, I need to invoke parse marketing command line through a stable signature so the features systems capture workflow remains explicit and composable. @fn FMarketingCaptureConfig ParseMarketingCommandLine( const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings) */
FMarketingCaptureConfig ParseMarketingCommandLine(
    const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings) {
  const FString DefaultOutputDirectory =
      ResolveCaptureOutputPath(Settings.DefaultOutputDirectory);
  FMarketingCaptureConfig Config;
  Config.Run.bQuitWhenDone =
      CommandLineParam(Settings.QuitWhenDoneCommandLineKey);
  Config.Run.OutputDirectory = CommandLineString(
      Settings.OutputDirectoryCommandLineKey, DefaultOutputDirectory);
  Config.Timing.InitialDelaySeconds = CommandLineFloat(
      Settings.InitialDelayCommandLineKey, Settings.InitialDelaySeconds);
  Config.Timing.SettleSeconds = CommandLineFloat(
      Settings.SettleSecondsCommandLineKey, Settings.SettleSeconds);
  Config.Timing.BetweenSeconds = CommandLineFloat(
      Settings.BetweenSecondsCommandLineKey, Settings.BetweenSeconds);
  CreateCaptureOutputDirectory(Config.Run.OutputDirectory);
  return Config;
}

} // namespace CaptureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
