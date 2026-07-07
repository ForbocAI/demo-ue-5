#pragma once

#include "Features/Components/Data/RuntimeSettings/UISettingsTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Runtime/RuntimeInteractionTypes.h"
#include "Features/Systems/Runtime/RuntimeLevelViewTypes.h"
#include "Features/Systems/Runtime/RuntimeStateTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"
#include "Core/ecs.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds);

FString RuntimeBudgetScreenshotDirectory(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

FString RuntimeBudgetScreenshotPath(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index);

int32 SelectIntFromBool(
    bool bValue,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

double SelectRuntimeMilliseconds(
    double Seconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

double SelectRuntimeElapsedMilliseconds(
    const FTimeInterval &Interval,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

int32 SelectRuntimeStatsFramesPerSecond(
    float DeltaSeconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

int32 SelectRuntimeStatsStackDepth(
    const ecs::FWorld &World,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

int32 SelectRuntimeStatsLodIndex(
    const FLodModelQuery &Query,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

int32 ClampRuntimeStatsLodIndex(
    const FLodClampRange &Range,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

int64 SelectMemoryMegabytes(
    uint64 Bytes,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

double SelectThreadMilliseconds(uint32 Cycles);

int32 SelectRuntimeGpuIndex(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params);

bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
