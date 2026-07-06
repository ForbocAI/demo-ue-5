#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/RuntimeSettings/UISettingsTypes.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

class UMaterialInterface;
class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSlice {

FRenderingState CreateInitialState();
const rtk::Slice<FRenderingState> &GetSlice();
void ApplyRuntimeProfile(
    UWorld *World,
    const FLevelRetroRenderProfile &Profile,
    const ForbocAI::Game::Data::FRenderingRuntimeSettings &RuntimeSettings);
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
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
