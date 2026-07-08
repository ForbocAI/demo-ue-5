#pragma once

#include "Features/Components/Data/Settings/UI/Types.h"
#include "Features/Systems/Rendering/Types.h"
#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Level/View/Types.h"
#include "Features/Systems/State/Types.h"
#include "Features/Systems/View/Spawn/Types.h"

class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingAdapters {

float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

double SelectRuntimeBudgetClockSeconds();

void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings,
    int32 Index);

FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

} // namespace RenderingAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
