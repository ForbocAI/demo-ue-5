#pragma once

#include "Features/Systems/Rendering/Stats/RenderingStatsTypes.h"
#include "Features/Systems/SystemsSelectors.h"

#include <cstdarg>

// Runtime stats selectors: pure functions that select and format runtime
// stats values for the overlay view. No Unreal IO, no widget calls.
// Views call these to get already-reduced presentation data.

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsSelectors {

using FStatsOverlaySettings =
    ForbocAI::Game::Data::FStatsOverlaySettings;

struct FFormatContext {
  const FString *Format;
  int32 BufferCharacterCount;
};

struct FRuntimeStatsMessageFormatRequest {
  const FRuntimeStatsViewModel *Stats;
  const FString *Format;
  const FStatsOverlaySettings *Settings;
};

inline const FStatsOverlaySettings &SelectStatsOverlaySettings() {
  return RuntimeSelectors::SelectUISettings(RuntimeSelectors::SelectState())
      .StatsOverlay;
}

inline FString FormatRuntimeStatsText(FFormatContext Context, ...) {
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
    int64 Value, const FStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
}

inline FString FormatRuntimeStatsDecimalValue(
    double Value, const FStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsText(
      {&Settings.DecimalValueFormat, Settings.FormatBufferCharacterCount},
      Value);
}

inline FString FormatRuntimeStatsMessage(
    const FRuntimeStatsMessageFormatRequest &Request) {
  const FRuntimeStatsViewModel &Stats = *Request.Stats;
  const FStatsOverlaySettings &Settings = *Request.Settings;
  return FormatRuntimeStatsText(
      {Request.Format, Settings.FormatBufferCharacterCount},
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

inline FLinearColor SelectRuntimeStatsValueColor(
    const FValueUpdateRequest &Request) {
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
    const FStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsMessage(
      {&Stats, &Settings.DebugMessageFormat, &Settings});
}

inline FString FormatRuntimeStatsBudgetLogMessage(
    const FRuntimeStatsViewModel &Stats,
    const FStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsMessage(
      {&Stats, &Settings.BudgetLogFormat, &Settings});
}

} // namespace RenderingStatsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
