#pragma once

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingAdapters {

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime budget screenshot interval seconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn float SelectRuntimeBudgetScreenshotIntervalSeconds( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime budget clock seconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn double SelectRuntimeBudgetClockSeconds() */
double SelectRuntimeBudgetClockSeconds();

/** User Story: As a systems rendering diagnostics consumer, I need to invoke request runtime budget screenshot through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn void RequestRuntimeBudgetScreenshot( const ForbocAI::Game::Data::FOverlaySettings &Settings, int32 Index) */
void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FOverlaySettings &Settings,
    int32 Index);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime memory stats through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FRuntimeMemoryStats SelectRuntimeMemoryStats( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime frame timing stats through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime poly count stats through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FRuntimePolyCountStats SelectRuntimePolyCountStats( UWorld *World, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime stats through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FRuntimeStatsViewModel SelectRuntimeStats( UWorld *World, float DeltaSeconds, double WallDeltaSeconds, int64 PolyCount, double PolyCountMilliseconds, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

} // namespace RenderingAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
