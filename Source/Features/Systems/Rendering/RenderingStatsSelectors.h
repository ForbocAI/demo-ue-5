#pragma once

#include "Features/Systems/Rendering/RenderingStatsTypes.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"

#include <cstdarg>

// Runtime stats selectors: pure functions that select and format runtime
// stats values for the overlay view. No Unreal IO, no widget calls.
// Views call these to get already-reduced presentation data.

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsSelectors {

using FRuntimeStatsOverlaySettings =
    ForbocAI::Game::Data::FRuntimeStatsOverlaySettings;

struct FStatsFormatContext {
  const FString *Format;
  int32 BufferCharacterCount;
};

inline const FRuntimeStatsOverlaySettings &SelectStatsOverlaySettings() {
  return RuntimeSelectors::SelectUIRuntimeSettings(RuntimeSelectors::SelectState())
      .StatsOverlay;
}

inline FString FormatRuntimeStatsText(FStatsFormatContext Context, ...) {
  TArray<TCHAR> Buffer;
  Buffer.SetNumZeroed(Context.BufferCharacterCount);
  const TCHAR *FormatPtr = **Context.Format;
  va_list Args;
  va_start(Args, Context);
  FCString::GetVarArgs(Buffer.GetData(), Buffer.Num(), FormatPtr, Args);
  va_end(Args);
  return FString(Buffer.GetData());
}

inline FString FormatRuntimeStatsValue(
    int64 Value, const FRuntimeStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
}

inline FString FormatRuntimeStatsDecimalValue(
    double Value, const FRuntimeStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsText(
      {&Settings.DecimalValueFormat, Settings.FormatBufferCharacterCount},
      Value);
}

inline FLinearColor SelectRuntimeStatsValueColor(
    const FStatsValueUpdateRequest &Request) {
  return Request.Value >= Request.HighThreshold
             ? Request.Settings->HighValueColor
             : (Request.Value >= Request.MediumThreshold
                    ? Request.Settings->MediumValueColor
                    : Request.Settings->LowValueColor);
}

inline bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) {
  return ElapsedSeconds >= IntervalSeconds;
}

inline FString FormatRuntimeStatsDebugMessage(
    const FRuntimeStatsViewModel &Stats,
    const FRuntimeStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsText(
      {&Settings.DebugMessageFormat, Settings.FormatBufferCharacterCount},
      Stats.FramesPerSecond, Stats.StackDepth,
      static_cast<long long>(Stats.PolyCount),
      static_cast<long long>(Stats.UsedPhysicalMemoryMegabytes),
      static_cast<long long>(Stats.PeakPhysicalMemoryMegabytes),
      static_cast<long long>(Stats.UsedVirtualMemoryMegabytes),
      Stats.GameThreadMilliseconds, Stats.RenderThreadMilliseconds,
      Stats.RhiThreadMilliseconds, Stats.GpuMilliseconds, Stats.DrawCalls,
      Stats.RhiPrimitives, Stats.WallDeltaMilliseconds,
      Stats.InputDeltaMilliseconds, Stats.StatsSelectionMilliseconds,
      Stats.PolyCountMilliseconds, Stats.EngineIdleMilliseconds,
      Stats.EngineIdleOvershootMilliseconds, Stats.MaxFps,
      Stats.FrameRateLimit, Stats.EffectiveMaxTickRate,
      Stats.FixedFrameRateEnabled, Stats.FixedFrameRate,
      Stats.FixedTimeStepEnabled, Stats.FixedDeltaMilliseconds,
      Stats.VsyncEnabled, Stats.IdleWhenNotForegroundEnabled,
      Stats.AppHasFocus, Stats.CpuThrottleEnabled, Stats.AllWindowsHidden,
      Stats.RootReducerMilliseconds, Stats.CombinedReducerMilliseconds,
      Stats.EcsProjectionMilliseconds, Stats.ProjectedEntityCount,
      Stats.ProjectedComponentTypeCount);
}

} // namespace RenderingStatsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
