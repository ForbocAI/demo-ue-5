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
 * @fn inline bool SelectCommandLineParam(const FString &Key)
 * @brief Returns true when the command line contains the given flag.
 *
 * Thin selector facade — delegates to CaptureAdapters::CommandLineParam.
 * User Story: As a features systems capture consumer, I need to invoke select command line param through a stable signature so the features systems capture workflow remains explicit and composable.
 */
inline bool SelectCommandLineParam(const FString &Key) {
  return CaptureAdapters::CommandLineParam(Key);
}

/**
 * @fn inline FScaleAuditCaptureConfig SelectScaleAuditCommandLineConfig( const ForbocAI::Game::Data::FAuditCaptureSettings &Settings, float TerrainWorldSize)
 * @brief Parses command-line overrides for scale-audit captures and
 * creates the output directory.
 *
 * Thin selector facade — delegates to CaptureAdapters::ParseScaleAuditCommandLine.
 * User Story: As a features systems capture consumer, I need to invoke select scale audit command line config through a stable signature so the features systems capture workflow remains explicit and composable.
 */
inline FScaleAuditCaptureConfig SelectScaleAuditCommandLineConfig(
    const ForbocAI::Game::Data::FAuditCaptureSettings &Settings,
    float TerrainWorldSize) {
  return CaptureAdapters::ParseScaleAuditCommandLine(Settings,
                                                     TerrainWorldSize);
}

/**
 * @fn inline FMarketingCaptureConfig SelectMarketingCommandLineConfig( const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings)
 * @brief Parses command-line overrides for marketing-brochure captures and
 * creates the output directory.
 *
 * Thin selector facade — delegates to CaptureAdapters::ParseMarketingCommandLine.
 * User Story: As a features systems capture consumer, I need to invoke select marketing command line config through a stable signature so the features systems capture workflow remains explicit and composable.
 */
inline FMarketingCaptureConfig SelectMarketingCommandLineConfig(
    const ForbocAI::Game::Data::FMarketingCaptureSettings &Settings) {
  return CaptureAdapters::ParseMarketingCommandLine(Settings);
}

} // namespace CaptureSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
