#pragma once

/**
 * @file Capture/Actions.h
 * @brief View-facing action facade for capture side effects.
 *
 * Views include this header instead of Capture/Adapters.h for
 * side-effect operations like requesting screenshots.
 * Keeps the adapter boundary hidden behind an Actions facade so the
 * View layer never imports adapter internals directly (RTK-VIEW-002).
 */

#include "Features/Systems/Capture/CaptureAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace CaptureActions {

/**
 * @fn inline void DispatchRequestCaptureScreenshot(const FString &OutputDir, const FString &OutputName)
 * @brief Requests a screenshot be written to the given path.
 *
 * Thin action facade — delegates to CaptureAdapters::RequestCaptureScreenshot.
 * User Story: As a features systems capture consumer, I need to invoke dispatch request capture screenshot through a stable signature so the features systems capture workflow remains explicit and composable.
 */
inline void DispatchRequestCaptureScreenshot(const FString &OutputDir,
                                             const FString &OutputName) {
  CaptureAdapters::RequestCaptureScreenshot(OutputDir, OutputName);
}

} // namespace CaptureActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
