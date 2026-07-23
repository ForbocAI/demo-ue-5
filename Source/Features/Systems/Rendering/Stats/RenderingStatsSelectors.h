#pragma once

#include "Features/Systems/Rendering/Stats/Widget/WidgetAdapters.h"
#include "Features/Systems/SystemsSelectors.h"

#include <cstdarg>

// Runtime stats selectors: pure functions that select and format runtime
// stats values for the overlay view. No Unreal IO, no widget calls.
// Views call these to get already-reduced presentation data.

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsSelectors {

using FOverlaySettings =
    ForbocAI::Game::Data::FOverlaySettings;

struct FFormatContext {
  const FString *Format;
  int32 BufferCharacterCount;
};

struct FRuntimeStatsMessageFormatRequest {
  const FRuntimeStatsViewModel *Stats;
  const FString *Format;
  const FOverlaySettings *Settings;
};

/** User Story: As a systems rendering stats consumer, I need to invoke select stats overlay settings through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline const FOverlaySettings &SelectStatsOverlaySettings() */
inline const FOverlaySettings &SelectStatsOverlaySettings() {
  return RuntimeSelectors::SelectUISettings(RuntimeSelectors::SelectState())
      .StatsOverlay;
}

/** User Story: As a systems rendering stats consumer, I need to invoke format runtime stats text through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FString FormatRuntimeStatsText(FFormatContext Context, ...) */
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

/** User Story: As a systems rendering stats consumer, I need to invoke format runtime stats value through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FString FormatRuntimeStatsValue( int64 Value, const FOverlaySettings &Settings) */
inline FString FormatRuntimeStatsValue(
    int64 Value, const FOverlaySettings &Settings) {
  return FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
}

/** User Story: As a systems rendering stats consumer, I need to invoke format runtime stats decimal value through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FString FormatRuntimeStatsDecimalValue( double Value, const FOverlaySettings &Settings) */
inline FString FormatRuntimeStatsDecimalValue(
    double Value, const FOverlaySettings &Settings) {
  return FormatRuntimeStatsText(
      {&Settings.DecimalValueFormat, Settings.FormatBufferCharacterCount},
      Value);
}

/** User Story: As a systems rendering stats consumer, I need to invoke format runtime stats message through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FString FormatRuntimeStatsMessage( const FRuntimeStatsMessageFormatRequest &Request) */
inline FString FormatRuntimeStatsMessage(
    const FRuntimeStatsMessageFormatRequest &Request) {
  const FRuntimeStatsViewModel &Stats = *Request.Stats;
  const FOverlaySettings &Settings = *Request.Settings;
  return FormatRuntimeStatsText(
      {Request.Format, Settings.FormatBufferCharacterCount},
      Stats.Summary.FramesPerSecond, Stats.Summary.StackDepth,
      static_cast<long long>(Stats.Summary.PolyCount),
      static_cast<long long>(Stats.Memory.UsedPhysicalMegabytes),
      static_cast<long long>(Stats.Memory.PeakUsedPhysicalMegabytes),
      static_cast<long long>(Stats.Memory.UsedVirtualMegabytes),
      Stats.Timing.GameThreadMilliseconds, Stats.Timing.RenderThreadMilliseconds,
      Stats.Timing.RhiThreadMilliseconds, Stats.Timing.GpuMilliseconds, Stats.Timing.DrawCalls,
      Stats.Timing.RhiPrimitives, Stats.Pacing.Delta.WallDeltaMilliseconds,
      Stats.Pacing.Delta.InputDeltaMilliseconds,
      Stats.Pacing.Delta.StatsSelectionMilliseconds,
      Stats.Pacing.Delta.PolyCountMilliseconds,
      Stats.Pacing.Delta.EngineIdleMilliseconds,
      Stats.Pacing.Delta.EngineIdleOvershootMilliseconds, Stats.Pacing.Rate.MaxFps,
      Stats.Pacing.Rate.FrameRateLimit, Stats.Pacing.Rate.EffectiveMaxTickRate,
      Stats.Pacing.Mode.FixedFrameRateEnabled,
      Stats.Pacing.Rate.FixedFrameRate,
      Stats.Pacing.Mode.FixedTimeStepEnabled, Stats.Pacing.Mode.FixedDeltaMilliseconds,
      Stats.Pacing.Mode.VsyncEnabled, Stats.Pacing.Mode.IdleWhenNotForegroundEnabled,
      Stats.Pacing.Mode.AppHasFocus, Stats.Pacing.Throttle.CpuThrottleEnabled, Stats.Pacing.Throttle.AllWindowsHidden,
      Stats.Reducers.RootReducerMilliseconds, Stats.Reducers.CombinedReducerMilliseconds,
      Stats.Projection.EcsProjectionMilliseconds, Stats.Projection.ProjectedEntityCount,
      Stats.Projection.ProjectedComponentTypeCount);
}

/** User Story: As a systems rendering stats consumer, I need to invoke select runtime stats value color through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FLinearColor SelectRuntimeStatsValueColor( const FValueUpdateRequest &Request) */
inline FLinearColor SelectRuntimeStatsValueColor(
    const FValueUpdateRequest &Request) {
  return Request.Value >= Request.HighThreshold
             ? Request.Settings->HighValueColor
             : (Request.Value >= Request.MediumThreshold
                    ? Request.Settings->MediumValueColor
                    : Request.Settings->LowValueColor);
}

/** User Story: As a systems rendering stats consumer, I need to invoke should run interval through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) */
inline bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) {
  return ElapsedSeconds >= IntervalSeconds;
}

/** User Story: As a systems rendering stats consumer, I need to invoke format runtime stats debug message through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FString FormatRuntimeStatsDebugMessage( const FRuntimeStatsViewModel &Stats, const FOverlaySettings &Settings) */
inline FString FormatRuntimeStatsDebugMessage(
    const FRuntimeStatsViewModel &Stats,
    const FOverlaySettings &Settings) {
  return FormatRuntimeStatsMessage(
      {&Stats, &Settings.DebugMessageFormat, &Settings});
}

/** User Story: As a systems rendering stats consumer, I need to invoke format runtime stats budget log message through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FString FormatRuntimeStatsBudgetLogMessage( const FRuntimeStatsViewModel &Stats, const FOverlaySettings &Settings) */
inline FString FormatRuntimeStatsBudgetLogMessage(
    const FRuntimeStatsViewModel &Stats,
    const FOverlaySettings &Settings) {
  return FormatRuntimeStatsMessage(
      {&Stats, &Settings.BudgetLogFormat, &Settings});
}

} // namespace RenderingStatsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
