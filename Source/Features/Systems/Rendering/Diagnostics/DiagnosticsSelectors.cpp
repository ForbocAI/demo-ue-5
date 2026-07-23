#include "Features/Systems/Rendering/Diagnostics/DiagnosticsSelectors.h"

#include "Core/fp.hpp"
#include "HAL/PlatformTime.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

/** User Story: As a systems rendering diagnostics consumer, I need to invoke should run interval through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) */
bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) {
  return ElapsedSeconds >= IntervalSeconds;
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select int from bool through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectIntFromBool( bool bValue, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectIntFromBool(
    bool bValue,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return bValue ? Settings.DiagnosticTrueIntValue
                : Settings.DiagnosticFalseIntValue;
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime milliseconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn double SelectRuntimeMilliseconds( double Seconds, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
double SelectRuntimeMilliseconds(
    double Seconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return Seconds * Settings.SecondsToMilliseconds;
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime elapsed milliseconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn double SelectRuntimeElapsedMilliseconds( const FTimeInterval &Interval, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
double SelectRuntimeElapsedMilliseconds(
    const FTimeInterval &Interval,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return SelectRuntimeMilliseconds(Interval.FinishedSeconds - Interval.StartedSeconds, Settings);
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime stats frames per second through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectRuntimeStatsFramesPerSecond( float DeltaSeconds, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectRuntimeStatsFramesPerSecond(
    float DeltaSeconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return DeltaSeconds > Settings.MinimumDeltaSeconds
             ? FMath::RoundToInt(Settings.FramesPerSecondNumerator /
                                 DeltaSeconds)
             : Settings.InitialFramesPerSecond;
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime stats stack depth through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectRuntimeStatsStackDepth( const ecs::FWorld &World, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectRuntimeStatsStackDepth(
    const ecs::FWorld &World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  TArray<ecs::FNode> Nodes;
  World.Domains.Nodes.GenerateValueArray(Nodes);
  return func::fold_array<ecs::FNode, int32>(
      Nodes, Settings.EmptyStackDepth,
      [](const int32 &Depth, const ecs::FNode &Node) {
        return FMath::Max(Depth, Node.Path.Segments.Num());
      });
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime stats lod index through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectRuntimeStatsLodIndex( const FLodModelQuery &Query, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectRuntimeStatsLodIndex(
    const FLodModelQuery &Query,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return Query.ForcedLodModel > Settings.ForcedLodAutomaticModel
             ? Query.ForcedLodModel - Settings.LodModelIndexOffset
             : Query.AutomaticLodIndex;
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke clamp runtime stats lod index through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 ClampRuntimeStatsLodIndex( const FLodClampRange &Range, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 ClampRuntimeStatsLodIndex(
    const FLodClampRange &Range,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return FMath::Clamp(Range.LodIndex, Settings.ForcedLodAutomaticModel,
                      Range.LodCount - Settings.LodModelIndexOffset);
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select memory megabytes through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int64 SelectMemoryMegabytes( uint64 Bytes, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int64 SelectMemoryMegabytes(
    uint64 Bytes,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  check(Settings.MemoryBytesPerMegabyte > Settings.EmptyMemoryMegabytes);
  return static_cast<int64>(
      Bytes / static_cast<uint64>(Settings.MemoryBytesPerMegabyte));
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select thread milliseconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn double SelectThreadMilliseconds(uint32 Cycles) */
double SelectThreadMilliseconds(uint32 Cycles) {
  return FPlatformTime::ToMilliseconds(Cycles);
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime gpu index through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectRuntimeGpuIndex( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
int32 SelectRuntimeGpuIndex(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return FMath::Clamp(Settings.RhiStatsGpuIndex,
                      Settings.RhiStatsMinimumGpuIndex,
                      Settings.RhiStatsMaximumGpuIndex);
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke should run runtime budget wall interval through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) */
bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) {
  return Params.IntervalSeconds > 0.0f &&
         Params.CurrentSeconds - Params.PreviousSeconds >= Params.IntervalSeconds;
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke should run runtime budget screenshot through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn bool ShouldRunRuntimeBudgetScreenshot( const FBudgetCheckParams &Params, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return Params.IntervalSeconds > Settings.BudgetScreenshotDisabledIntervalSeconds &&
         ShouldRunRuntimeBudgetWallInterval(Params);
}

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
