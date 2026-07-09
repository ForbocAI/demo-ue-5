#pragma once

/**
 * @file Capture/Selectors.h
 * @brief View-facing selector facade for capture configuration.
 *
 * Views include this header instead of Capture/Adapters.h.
 * Keeps the adapter boundary hidden behind a Selectors facade so the
 * View layer never imports adapter internals directly (RTK-VIEW-002).
 */

#include "Features/Systems/Capture/CaptureAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace CaptureSelectors {

// Re-export config POD types so Views can name them without touching Adapters.
using FScaleAuditCaptureConfig = CaptureAdapters::FScaleAuditCaptureConfig;
using FMarketingCaptureConfig = CaptureAdapters::FMarketingCaptureConfig;

/**
 * @brief Returns true when the command line contains the given flag.
 *
 * Thin selector facade — delegates to CaptureAdapters::CommandLineParam.
 */
inline bool SelectCommandLineParam(const FString &Key) {
  return CaptureAdapters::CommandLineParam(Key);
}

/**
 * @brief Parses command-line overrides for scale-audit captures and
 *        creates the output directory.
 *
 * Thin selector facade — delegates to CaptureAdapters::ParseScaleAuditCommandLine.
 */
inline FScaleAuditCaptureConfig SelectScaleAuditCommandLineConfig(
    const ForbocAI::Game::Data::FAuditCaptureSettings &Settings,
    float TerrainWorldSize) {
  return CaptureAdapters::ParseScaleAuditCommandLine(Settings,
                                                     TerrainWorldSize);
}

/**
 * @brief Parses command-line overrides for marketing-brochure captures and
 *        creates the output directory.
 *
 * Thin selector facade — delegates to CaptureAdapters::ParseMarketingCommandLine.
 */
inline FMarketingCaptureConfig SelectMarketingCommandLineConfig(
    const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings) {
  return CaptureAdapters::ParseMarketingCommandLine(Settings);
}

} // namespace CaptureSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
