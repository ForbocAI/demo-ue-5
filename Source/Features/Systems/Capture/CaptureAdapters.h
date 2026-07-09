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
 * @brief Reads a float from the command line, falling back to Fallback.
 * @signature float CommandLineFloat(Key, Fallback)
 *
 * Wraps FParse::Value(FCommandLine::Get(), ...) so callers never touch IO.
 */
float CommandLineFloat(const FString &Key, float Fallback);

/**
 * @brief Reads a string from the command line, falling back to Fallback.
 * @signature FString CommandLineString(Key, Fallback)
 */
FString CommandLineString(const FString &Key, const FString &Fallback);

/**
 * @brief Returns true when the command line contains the given flag.
 * @signature bool CommandLineParam(Key)
 */
bool CommandLineParam(const FString &Key);

/**
 * @brief Resolves a project-relative default output path to a full path.
 * @signature FString ResolveCaptureOutputPath(RelativeDir)
 *
 * Wraps FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), ...)).
 */
FString ResolveCaptureOutputPath(const FString &RelativeDir);

/**
 * @brief Creates the output directory on disk (recursive).
 * @signature void CreateCaptureOutputDirectory(Directory)
 *
 * Wraps IFileManager::Get().MakeDirectory(*Directory, true).
 */
void CreateCaptureOutputDirectory(const FString &Directory);

/**
 * @brief Requests a screenshot be written to the given path.
 * @signature void RequestCaptureScreenshot(OutputDir, OutputName)
 *
 * Wraps FPaths::Combine + FScreenshotRequest::RequestScreenshot.
 */
void RequestCaptureScreenshot(const FString &OutputDir,
                              const FString &OutputName);

/**
 * @brief Parses command-line overrides for scale-audit captures.
 * @signature FScaleAuditCaptureConfig ParseScaleAuditCommandLine(Settings, TerrainWorldSize)
 *
 * Reads all -Key=Value overrides from the command line and returns a
 * fully-resolved config struct. Also creates the output directory.
 */
FScaleAuditCaptureConfig ParseScaleAuditCommandLine(
    const ForbocAI::Game::Data::FAuditCaptureSettings &Settings,
    float TerrainWorldSize);

/**
 * @brief Parses command-line overrides for marketing-brochure captures.
 * @signature FMarketingCaptureConfig ParseMarketingCommandLine(Settings)
 *
 * Reads all -Key=Value overrides from the command line and returns a
 * fully-resolved config struct. Also creates the output directory.
 */
FMarketingCaptureConfig ParseMarketingCommandLine(
    const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings);

} // namespace CaptureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
