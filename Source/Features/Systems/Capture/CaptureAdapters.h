#pragma once

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Game {

namespace Data {
struct FAuditCaptureSettings;
struct FMarketingCaptureSettings;
} // namespace Data

namespace Level {
namespace CaptureAdapters {

struct FRunConfig {
  bool bQuitWhenDone = false;
  FString OutputDirectory;
};

struct FTimingConfig {
  float InitialDelaySeconds = float();
  float SettleSeconds = float();
  float BetweenSeconds = float();
};

struct FScaleAuditOrthoConfig {
  float WholeWidth = float();
  float TownWidth = float();
  float ActorsWidth = float();
};

struct FScaleAuditHeightConfig {
  float WholeHeight = float();
  float TownHeight = float();
  float ActorsHeight = float();
};

/**
 * @brief Resolved command-line configuration for scale-audit captures.
 *
 * The View consumes this struct instead of calling FParse / FCommandLine
 * directly, keeping IO behind the adapter boundary (RTK-VIEW-008).
 */
struct FScaleAuditCaptureConfig {
  FRunConfig Run;
  FTimingConfig Timing;
  FScaleAuditOrthoConfig Ortho;
  FScaleAuditHeightConfig Height;
};

/**
 * @brief Resolved command-line configuration for marketing-brochure captures.
 */
struct FMarketingCaptureConfig {
  FRunConfig Run;
  FTimingConfig Timing;
};

/**
 * @fn float CommandLineFloat(const FString &Key, float Fallback)
 * @brief Reads a float from the command line, falling back to Fallback.
 *
 * Wraps FParse::Value(FCommandLine::Get(), ...) so callers never touch IO.
 * User Story: As a features systems capture consumer, I need to invoke command line float through a stable signature so the features systems capture workflow remains explicit and composable.
 */
float CommandLineFloat(const FString &Key, float Fallback);

/**
 * @fn FString CommandLineString(const FString &Key, const FString &Fallback)
 * @brief Reads a string from the command line, falling back to Fallback.
 * User Story: As a features systems capture consumer, I need to invoke command line string through a stable signature so the features systems capture workflow remains explicit and composable.
 */
FString CommandLineString(const FString &Key, const FString &Fallback);

/**
 * @fn bool CommandLineParam(const FString &Key)
 * @brief Returns true when the command line contains the given flag.
 * User Story: As a features systems capture consumer, I need to invoke command line param through a stable signature so the features systems capture workflow remains explicit and composable.
 */
bool CommandLineParam(const FString &Key);

/**
 * @fn FString ResolveCaptureOutputPath(const FString &RelativeDir)
 * @brief Resolves a project-relative default output path to a full path.
 *
 * Wraps FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), ...)).
 * User Story: As a features systems capture consumer, I need to invoke resolve capture output path through a stable signature so the features systems capture workflow remains explicit and composable.
 */
FString ResolveCaptureOutputPath(const FString &RelativeDir);

/**
 * @fn void CreateCaptureOutputDirectory(const FString &Directory)
 * @brief Creates the output directory on disk (recursive).
 *
 * Wraps IFileManager::Get().MakeDirectory(*Directory, true).
 * User Story: As a features systems capture consumer, I need to invoke create capture output directory through a stable signature so the features systems capture workflow remains explicit and composable.
 */
void CreateCaptureOutputDirectory(const FString &Directory);

/**
 * @fn void RequestCaptureScreenshot(const FString &OutputDir, const FString &OutputName)
 * @brief Requests a screenshot be written to the given path.
 *
 * Wraps FPaths::Combine + FScreenshotRequest::RequestScreenshot.
 * User Story: As a features systems capture consumer, I need to invoke request capture screenshot through a stable signature so the features systems capture workflow remains explicit and composable.
 */
void RequestCaptureScreenshot(const FString &OutputDir,
                              const FString &OutputName);

/**
 * @fn FScaleAuditCaptureConfig ParseScaleAuditCommandLine( const ForbocAI::Game::Data::FAuditCaptureSettings &Settings, float TerrainWorldSize)
 * @brief Parses command-line overrides for scale-audit captures.
 *
 * Reads all -Key=Value overrides from the command line and returns a
 * fully-resolved config struct. Also creates the output directory.
 * User Story: As a features systems capture consumer, I need to invoke parse scale audit command line through a stable signature so the features systems capture workflow remains explicit and composable.
 */
FScaleAuditCaptureConfig ParseScaleAuditCommandLine(
    const ForbocAI::Game::Data::FAuditCaptureSettings &Settings,
    float TerrainWorldSize);

/**
 * @fn FMarketingCaptureConfig ParseMarketingCommandLine( const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings)
 * @brief Parses command-line overrides for marketing-brochure captures.
 *
 * Reads all -Key=Value overrides from the command line and returns a
 * fully-resolved config struct. Also creates the output directory.
 * User Story: As a features systems capture consumer, I need to invoke parse marketing command line through a stable signature so the features systems capture workflow remains explicit and composable.
 */
FMarketingCaptureConfig ParseMarketingCommandLine(
    const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings);

} // namespace CaptureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
