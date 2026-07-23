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

/** User Story: As a systems rendering diagnostics consumer, I need to invoke should run interval through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) */
bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select int from bool through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectIntFromBool( bool bValue, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectIntFromBool(
    bool bValue,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime milliseconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn double SelectRuntimeMilliseconds( double Seconds, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
double SelectRuntimeMilliseconds(
    double Seconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime elapsed milliseconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn double SelectRuntimeElapsedMilliseconds( const FTimeInterval &Interval, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
double SelectRuntimeElapsedMilliseconds(
    const FTimeInterval &Interval,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime stats frames per second through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectRuntimeStatsFramesPerSecond( float DeltaSeconds, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectRuntimeStatsFramesPerSecond(
    float DeltaSeconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime stats stack depth through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectRuntimeStatsStackDepth( const ecs::FWorld &World, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectRuntimeStatsStackDepth(
    const ecs::FWorld &World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime stats lod index through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectRuntimeStatsLodIndex( const FLodModelQuery &Query, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectRuntimeStatsLodIndex(
    const FLodModelQuery &Query,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke clamp runtime stats lod index through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 ClampRuntimeStatsLodIndex( const FLodClampRange &Range, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 ClampRuntimeStatsLodIndex(
    const FLodClampRange &Range,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select memory megabytes through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int64 SelectMemoryMegabytes( uint64 Bytes, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int64 SelectMemoryMegabytes(
    uint64 Bytes,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select thread milliseconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn double SelectThreadMilliseconds(uint32 Cycles) */
double SelectThreadMilliseconds(uint32 Cycles);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime gpu index through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectRuntimeGpuIndex( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectRuntimeGpuIndex(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke should run runtime budget wall interval through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) */
bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params);

/** User Story: As a systems rendering diagnostics consumer, I need to invoke should run runtime budget screenshot through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn bool ShouldRunRuntimeBudgetScreenshot( const FBudgetCheckParams &Params, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
