#pragma once

#include "Features/Components/Data/Settings/UI/Types.h"
#include "Features/Systems/Rendering/Types.h"
#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Level/View/Types.h"
#include "Features/Systems/State/Types.h"
#include "Features/Systems/View/Spawn/Types.h"
#include "Core/ecs.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds);

int32 SelectIntFromBool(
    bool bValue,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

double SelectRuntimeMilliseconds(
    double Seconds,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

double SelectRuntimeElapsedMilliseconds(
    const FTimeInterval &Interval,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

int32 SelectRuntimeStatsFramesPerSecond(
    float DeltaSeconds,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

int32 SelectRuntimeStatsStackDepth(
    const ecs::FWorld &World,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

int32 SelectRuntimeStatsLodIndex(
    const FLodModelQuery &Query,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

int32 ClampRuntimeStatsLodIndex(
    const FLodClampRange &Range,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

int64 SelectMemoryMegabytes(
    uint64 Bytes,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

double SelectThreadMilliseconds(uint32 Cycles);

int32 SelectRuntimeGpuIndex(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params);

bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
