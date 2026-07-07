#include "Features/Systems/Rendering/RenderingDiagnosticsSelectors.h"

#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "HAL/PlatformTime.h"
#include "Misc/Paths.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) {
  return ElapsedSeconds >= IntervalSeconds;
}

FString RuntimeBudgetScreenshotDirectory(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return FPaths::Combine(FPaths::ProjectDir(),
                         Settings.BudgetScreenshotDirectory);
}

FString RuntimeBudgetScreenshotPath(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index) {
  return FPaths::Combine(
      RuntimeBudgetScreenshotDirectory(Settings),
      frmt::RuntimeString(Settings.BudgetScreenshotFileNameFormat,
                          frmt::Args({frmt::Arg(Index)})));
}

int32 SelectIntFromBool(
    bool bValue,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return bValue ? Settings.DiagnosticTrueIntValue
                : Settings.DiagnosticFalseIntValue;
}

double SelectRuntimeMilliseconds(
    double Seconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return Seconds * Settings.SecondsToMilliseconds;
}

double SelectRuntimeElapsedMilliseconds(
    const FTimeInterval &Interval,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return SelectRuntimeMilliseconds(Interval.FinishedSeconds - Interval.StartedSeconds, Settings);
}

int32 SelectRuntimeStatsFramesPerSecond(
    float DeltaSeconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return DeltaSeconds > Settings.MinimumDeltaSeconds
             ? FMath::RoundToInt(Settings.FramesPerSecondNumerator /
                                 DeltaSeconds)
             : Settings.InitialFramesPerSecond;
}

int32 SelectRuntimeStatsStackDepth(
    const ecs::FWorld &World,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  TArray<ecs::FDomainNode> Nodes;
  World.Domains.Nodes.GenerateValueArray(Nodes);
  return func::fold_array<ecs::FDomainNode, int32>(
      Nodes, Settings.EmptyStackDepth,
      [](const int32 &Depth, const ecs::FDomainNode &Node) {
        return FMath::Max(Depth, Node.Path.Segments.Num());
      });
}

int32 SelectRuntimeStatsLodIndex(
    const FLodModelQuery &Query,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return Query.ForcedLodModel > Settings.ForcedLodAutomaticModel
             ? Query.ForcedLodModel - Settings.LodModelIndexOffset
             : Query.AutomaticLodIndex;
}

int32 ClampRuntimeStatsLodIndex(
    const FLodClampRange &Range,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return FMath::Clamp(Range.LodIndex, Settings.ForcedLodAutomaticModel,
                      Range.LodCount - Settings.LodModelIndexOffset);
}

int64 SelectMemoryMegabytes(
    uint64 Bytes,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  check(Settings.MemoryBytesPerMegabyte > Settings.EmptyMemoryMegabytes);
  return static_cast<int64>(
      Bytes / static_cast<uint64>(Settings.MemoryBytesPerMegabyte));
}

double SelectThreadMilliseconds(uint32 Cycles) {
  return FPlatformTime::ToMilliseconds(Cycles);
}

int32 SelectRuntimeGpuIndex(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return FMath::Clamp(Settings.RhiStatsGpuIndex,
                      Settings.RhiStatsMinimumGpuIndex,
                      Settings.RhiStatsMaximumGpuIndex);
}

bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) {
  return Params.IntervalSeconds > 0.0f &&
         Params.CurrentSeconds - Params.LastSeconds >= Params.IntervalSeconds;
}

bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return Params.IntervalSeconds > Settings.BudgetScreenshotDisabledIntervalSeconds &&
         ShouldRunRuntimeBudgetWallInterval(Params);
}

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
