#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeMemoryStats {
  int64 UsedPhysicalMegabytes;
  int64 PeakUsedPhysicalMegabytes;
  int64 UsedVirtualMegabytes;
};

struct FRuntimeFrameTimingStats {
  double GameThreadMilliseconds;
  double RenderThreadMilliseconds;
  double RhiThreadMilliseconds;
  double GpuMilliseconds;
  int32 DrawCalls;
  int32 RhiPrimitives;
};

struct FRuntimeFramePacingStats {
  double WallDeltaMilliseconds;
  double InputDeltaMilliseconds;
  double StatsSelectionMilliseconds;
  double PolyCountMilliseconds;
  double EngineIdleMilliseconds;
  double EngineIdleOvershootMilliseconds;
  float MaxFps;
  float FrameRateLimit;
  float EffectiveMaxTickRate;
  int32 FixedFrameRateEnabled;
  float FixedFrameRate;
  int32 FixedTimeStepEnabled;
  double FixedDeltaMilliseconds;
  int32 VsyncEnabled;
  int32 IdleWhenNotForegroundEnabled;
  int32 AppHasFocus;
  int32 CpuThrottleEnabled;
  int32 AllWindowsHidden;
};

struct FRuntimePolyCountStats {
  int64 PolyCount;
  double MeasurementMilliseconds;
};

// Groups a start/finish pair for elapsed-time selectors.
struct FTimeInterval {
  double StartedSeconds;
  double FinishedSeconds;
};

// Groups the two LOD model inputs for SelectRuntimeStatsLodIndex.
struct FLodModelQuery {
  int32 ForcedLodModel;
  int32 AutomaticLodIndex;
};

// Groups the LOD index and count for ClampRuntimeStatsLodIndex.
struct FLodClampRange {
  int32 LodIndex;
  int32 LodCount;
};

// Groups the 4 timing inputs for SelectRuntimeFramePacingStats.
struct FFramePacingQuery {
  float DeltaSeconds;
  double WallDeltaSeconds;
  double StatsSelectionMilliseconds;
  double PolyCountMilliseconds;
};

// Groups the wall-clock interval parameters for budget checks.
struct FBudgetCheckParams {
  double CurrentSeconds;
  double PreviousSeconds;
  float IntervalSeconds;
};

struct FRuntimeStatsViewModel {
  int32 FramesPerSecond;
  int32 StackDepth;
  int64 PolyCount;
  int64 UsedPhysicalMemoryMegabytes;
  int64 PeakPhysicalMemoryMegabytes;
  int64 UsedVirtualMemoryMegabytes;
  double GameThreadMilliseconds;
  double RenderThreadMilliseconds;
  double RhiThreadMilliseconds;
  double GpuMilliseconds;
  int32 DrawCalls;
  int32 RhiPrimitives;
  double WallDeltaMilliseconds;
  double InputDeltaMilliseconds;
  double StatsSelectionMilliseconds;
  double PolyCountMilliseconds;
  double EngineIdleMilliseconds;
  double EngineIdleOvershootMilliseconds;
  float MaxFps;
  float FrameRateLimit;
  float EffectiveMaxTickRate;
  int32 FixedFrameRateEnabled;
  float FixedFrameRate;
  int32 FixedTimeStepEnabled;
  double FixedDeltaMilliseconds;
  int32 VsyncEnabled;
  int32 IdleWhenNotForegroundEnabled;
  int32 AppHasFocus;
  int32 CpuThrottleEnabled;
  int32 AllWindowsHidden;
  double RootReducerMilliseconds;
  double CombinedReducerMilliseconds;
  double EcsProjectionMilliseconds;
  int32 ProjectedEntityCount;
  int32 ProjectedComponentTypeCount;
};

struct FStatsTextModel {
  FString Text;
  FLinearColor Color;
};

inline bool operator==(const FStatsTextModel &Left, const FStatsTextModel &Right) {
  return Left.Text == Right.Text && Left.Color == Right.Color;
}

inline bool operator!=(const FStatsTextModel &Left, const FStatsTextModel &Right) {
  return !(Left == Right);
}

struct FRuntimeStatsPresentationModel {
  FStatsTextModel FramesPerSecond;
  FStatsTextModel StackDepth;
  FStatsTextModel PolyCount;
  FStatsTextModel UsedPhysicalMemoryMegabytes;
  FStatsTextModel PeakPhysicalMemoryMegabytes;
  FStatsTextModel UsedVirtualMemoryMegabytes;
  FStatsTextModel GameThreadMilliseconds;
  FStatsTextModel RenderThreadMilliseconds;
  FStatsTextModel RhiThreadMilliseconds;
  FStatsTextModel GpuMilliseconds;
  FStatsTextModel DrawCalls;
  FStatsTextModel RhiPrimitives;
  FStatsTextModel WallDeltaMilliseconds;
  FStatsTextModel InputDeltaMilliseconds;
  FStatsTextModel StatsSelectionMilliseconds;
  FStatsTextModel PolyCountMilliseconds;
  FStatsTextModel EngineIdleMilliseconds;
  FStatsTextModel EngineIdleOvershootMilliseconds;
  FStatsTextModel MaxFps;
  FStatsTextModel FrameRateLimit;
  FStatsTextModel EffectiveMaxTickRate;
  FStatsTextModel FixedFrameRateEnabled;
  FStatsTextModel FixedFrameRate;
  FStatsTextModel FixedTimeStepEnabled;
  FStatsTextModel FixedDeltaMilliseconds;
  FStatsTextModel VsyncEnabled;
  FStatsTextModel IdleWhenNotForegroundEnabled;
  FStatsTextModel AppHasFocus;
  FStatsTextModel CpuThrottleEnabled;
  FStatsTextModel AllWindowsHidden;
  FStatsTextModel RootReducerMilliseconds;
  FStatsTextModel CombinedReducerMilliseconds;
  FStatsTextModel EcsProjectionMilliseconds;
  FStatsTextModel ProjectedEntityCount;
  FStatsTextModel ProjectedComponentTypeCount;
};

inline bool operator==(const FRuntimeStatsPresentationModel &Left, const FRuntimeStatsPresentationModel &Right) {
  return Left.FramesPerSecond == Right.FramesPerSecond &&
         Left.StackDepth == Right.StackDepth &&
         Left.PolyCount == Right.PolyCount &&
         Left.UsedPhysicalMemoryMegabytes == Right.UsedPhysicalMemoryMegabytes &&
         Left.PeakPhysicalMemoryMegabytes == Right.PeakPhysicalMemoryMegabytes &&
         Left.UsedVirtualMemoryMegabytes == Right.UsedVirtualMemoryMegabytes &&
         Left.GameThreadMilliseconds == Right.GameThreadMilliseconds &&
         Left.RenderThreadMilliseconds == Right.RenderThreadMilliseconds &&
         Left.RhiThreadMilliseconds == Right.RhiThreadMilliseconds &&
         Left.GpuMilliseconds == Right.GpuMilliseconds &&
         Left.DrawCalls == Right.DrawCalls &&
         Left.RhiPrimitives == Right.RhiPrimitives &&
         Left.WallDeltaMilliseconds == Right.WallDeltaMilliseconds &&
         Left.InputDeltaMilliseconds == Right.InputDeltaMilliseconds &&
         Left.StatsSelectionMilliseconds == Right.StatsSelectionMilliseconds &&
         Left.PolyCountMilliseconds == Right.PolyCountMilliseconds &&
         Left.EngineIdleMilliseconds == Right.EngineIdleMilliseconds &&
         Left.EngineIdleOvershootMilliseconds == Right.EngineIdleOvershootMilliseconds &&
         Left.MaxFps == Right.MaxFps &&
         Left.FrameRateLimit == Right.FrameRateLimit &&
         Left.EffectiveMaxTickRate == Right.EffectiveMaxTickRate &&
         Left.FixedFrameRateEnabled == Right.FixedFrameRateEnabled &&
         Left.FixedFrameRate == Right.FixedFrameRate &&
         Left.FixedTimeStepEnabled == Right.FixedTimeStepEnabled &&
         Left.FixedDeltaMilliseconds == Right.FixedDeltaMilliseconds &&
         Left.VsyncEnabled == Right.VsyncEnabled &&
         Left.IdleWhenNotForegroundEnabled == Right.IdleWhenNotForegroundEnabled &&
         Left.AppHasFocus == Right.AppHasFocus &&
         Left.CpuThrottleEnabled == Right.CpuThrottleEnabled &&
         Left.AllWindowsHidden == Right.AllWindowsHidden &&
         Left.RootReducerMilliseconds == Right.RootReducerMilliseconds &&
         Left.CombinedReducerMilliseconds == Right.CombinedReducerMilliseconds &&
         Left.EcsProjectionMilliseconds == Right.EcsProjectionMilliseconds &&
         Left.ProjectedEntityCount == Right.ProjectedEntityCount &&
         Left.ProjectedComponentTypeCount == Right.ProjectedComponentTypeCount;
}

inline bool operator!=(const FRuntimeStatsPresentationModel &Left, const FRuntimeStatsPresentationModel &Right) {
  return !(Left == Right);
}

struct FRuntimeStatsSamplePayload {
  float DeltaSeconds;
  double BudgetClockSeconds;
  double BudgetLogPreviousSeconds;
  double BudgetScreenshotPreviousSeconds;
  int32 BudgetScreenshotIndex;
  func::Maybe<FRuntimePolyCountStats> PolyCount;
  func::Maybe<FRuntimeStatsViewModel> Stats;
  ForbocAI::Game::Data::FStatsOverlaySettings StatsOverlay;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
