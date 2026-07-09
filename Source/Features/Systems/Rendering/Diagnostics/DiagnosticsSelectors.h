#pragma once

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"
#include "Core/ecs.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds);

int32 SelectIntFromBool(
    bool bValue,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

double SelectRuntimeMilliseconds(
    double Seconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

double SelectRuntimeElapsedMilliseconds(
    const FTimeInterval &Interval,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

int32 SelectRuntimeStatsFramesPerSecond(
    float DeltaSeconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

int32 SelectRuntimeStatsStackDepth(
    const ecs::FWorld &World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

int32 SelectRuntimeStatsLodIndex(
    const FLodModelQuery &Query,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

int32 ClampRuntimeStatsLodIndex(
    const FLodClampRange &Range,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

int64 SelectMemoryMegabytes(
    uint64 Bytes,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

double SelectThreadMilliseconds(uint32 Cycles);

int32 SelectRuntimeGpuIndex(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params);

bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
