#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/RuntimeSettings/UISettingsTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Runtime/RuntimeTypes.h"

class UMaterialInterface;
class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {

namespace RenderingThunks {

rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id);

void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request);
float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);
double SelectRuntimeBudgetClockSeconds();
bool ShouldRunRuntimeBudgetWallInterval(double CurrentSeconds,
                                        double LastSeconds,
                                        float IntervalSeconds);
bool ShouldRunRuntimeBudgetScreenshot(
    double CurrentSeconds, double LastSeconds, float IntervalSeconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);
void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index);
FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);
FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);
FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);
FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
