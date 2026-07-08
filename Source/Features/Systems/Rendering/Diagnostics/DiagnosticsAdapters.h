#pragma once

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/View/ViewTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/View/Spawn/ViewSpawnTypes.h"

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
