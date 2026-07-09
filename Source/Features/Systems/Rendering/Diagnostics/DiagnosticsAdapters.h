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

float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

double SelectRuntimeBudgetClockSeconds();

void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FOverlaySettings &Settings,
    int32 Index);

FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

} // namespace RenderingAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
