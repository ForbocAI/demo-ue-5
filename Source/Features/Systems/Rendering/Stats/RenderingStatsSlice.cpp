#include "Features/Systems/Rendering/SystemsRenderingSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingReducers {

struct FFormatContext {
  const FString *Format;
  int32 BufferCharacterCount;
};

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

inline FStatsTextModel ReduceRuntimeStatsValue(
    int64 Value, int32 MediumThreshold, int32 HighThreshold,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
  Model.Color =
      Value >= HighThreshold
          ? Settings.HighValueColor
          : (Value >= MediumThreshold ? Settings.MediumValueColor
                                      : Settings.LowValueColor);
  return Model;
}

inline FStatsTextModel ReduceRuntimeStatsDecimalValue(
    double Value,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.DecimalValueFormat, Settings.FormatBufferCharacterCount},
      Value);
  Model.Color = Settings.TextColor;
  return Model;
}

inline FStatsTextModel ReduceRuntimeStatsPlainValue(
    int32 Value,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
  Model.Color = Settings.TextColor;
  return Model;
}

inline FStatsTextModel ReduceRuntimeStatsPlainValue(
    int64 Value,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
  Model.Color = Settings.TextColor;
  return Model;
}

inline FRuntimeStatsPresentationModel ReduceRuntimeStatsPresentationModel(
    const FRuntimeStatsViewModel &Stats,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings) {
  FRuntimeStatsPresentationModel Presentation;
  Presentation.FramesPerSecond = ReduceRuntimeStatsValue(
      Stats.FramesPerSecond, Settings.FramesPerSecondMediumThreshold,
      Settings.FramesPerSecondHighThreshold, Settings);
  Presentation.StackDepth = ReduceRuntimeStatsValue(
      Stats.StackDepth, Settings.StackDepthMediumThreshold,
      Settings.StackDepthHighThreshold, Settings);
  Presentation.PolyCount =
      ReduceRuntimeStatsValue(Stats.PolyCount,
                              Settings.PolyCountMediumThreshold,
                              Settings.PolyCountHighThreshold, Settings);
  Presentation.UsedPhysicalMemoryMegabytes = ReduceRuntimeStatsValue(
      Stats.UsedPhysicalMemoryMegabytes, Settings.MemoryMediumThreshold,
      Settings.MemoryHighThreshold, Settings);
  Presentation.PeakPhysicalMemoryMegabytes = ReduceRuntimeStatsValue(
      Stats.PeakPhysicalMemoryMegabytes, Settings.MemoryMediumThreshold,
      Settings.MemoryHighThreshold, Settings);
  Presentation.UsedVirtualMemoryMegabytes = ReduceRuntimeStatsValue(
      Stats.UsedVirtualMemoryMegabytes, Settings.MemoryMediumThreshold,
      Settings.MemoryHighThreshold, Settings);
  Presentation.GameThreadMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.GameThreadMilliseconds, Settings);
  Presentation.RenderThreadMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.RenderThreadMilliseconds, Settings);
  Presentation.RhiThreadMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.RhiThreadMilliseconds, Settings);
  Presentation.GpuMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.GpuMilliseconds, Settings);
  Presentation.DrawCalls =
      ReduceRuntimeStatsPlainValue(Stats.DrawCalls, Settings);
  Presentation.RhiPrimitives =
      ReduceRuntimeStatsPlainValue(Stats.RhiPrimitives, Settings);
  Presentation.WallDeltaMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.WallDeltaMilliseconds, Settings);
  Presentation.InputDeltaMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.InputDeltaMilliseconds, Settings);
  Presentation.StatsSelectionMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.StatsSelectionMilliseconds, Settings);
  Presentation.PolyCountMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.PolyCountMilliseconds, Settings);
  Presentation.EngineIdleMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.EngineIdleMilliseconds, Settings);
  Presentation.EngineIdleOvershootMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.EngineIdleOvershootMilliseconds,
                                     Settings);
  Presentation.MaxFps =
      ReduceRuntimeStatsDecimalValue(Stats.MaxFps, Settings);
  Presentation.FrameRateLimit =
      ReduceRuntimeStatsDecimalValue(Stats.FrameRateLimit, Settings);
  Presentation.EffectiveMaxTickRate =
      ReduceRuntimeStatsDecimalValue(Stats.EffectiveMaxTickRate, Settings);
  Presentation.FixedFrameRateEnabled =
      ReduceRuntimeStatsPlainValue(Stats.FixedFrameRateEnabled, Settings);
  Presentation.FixedFrameRate =
      ReduceRuntimeStatsDecimalValue(Stats.FixedFrameRate, Settings);
  Presentation.FixedTimeStepEnabled =
      ReduceRuntimeStatsPlainValue(Stats.FixedTimeStepEnabled, Settings);
  Presentation.FixedDeltaMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.FixedDeltaMilliseconds, Settings);
  Presentation.VsyncEnabled =
      ReduceRuntimeStatsPlainValue(Stats.VsyncEnabled, Settings);
  Presentation.IdleWhenNotForegroundEnabled =
      ReduceRuntimeStatsPlainValue(Stats.IdleWhenNotForegroundEnabled, Settings);
  Presentation.AppHasFocus =
      ReduceRuntimeStatsPlainValue(Stats.AppHasFocus, Settings);
  Presentation.CpuThrottleEnabled =
      ReduceRuntimeStatsPlainValue(Stats.CpuThrottleEnabled, Settings);
  Presentation.AllWindowsHidden =
      ReduceRuntimeStatsPlainValue(Stats.AllWindowsHidden, Settings);
  Presentation.RootReducerMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.RootReducerMilliseconds, Settings);
  Presentation.CombinedReducerMilliseconds = ReduceRuntimeStatsDecimalValue(
      Stats.CombinedReducerMilliseconds, Settings);
  Presentation.EcsProjectionMilliseconds = ReduceRuntimeStatsDecimalValue(
      Stats.EcsProjectionMilliseconds, Settings);
  Presentation.ProjectedEntityCount =
      ReduceRuntimeStatsPlainValue(Stats.ProjectedEntityCount, Settings);
  Presentation.ProjectedComponentTypeCount =
      ReduceRuntimeStatsPlainValue(Stats.ProjectedComponentTypeCount, Settings);
  return Presentation;
}

FRenderingState ReduceRuntimeStatsSampled(
    const FRenderingState &State,
    const rtk::PayloadAction<FRuntimeStatsSamplePayload> &Action) {
  return (func::pipe(State) |
          [&Action, &State](FRenderingState Next) -> FRenderingState {
            const auto &Payload = Action.PayloadValue;
            const auto &Settings = Payload.StatsOverlay;

            Next.StatsClock.FrameClockSeconds = Payload.BudgetClockSeconds;
            Next.BudgetClock.BudgetLogPreviousSeconds =
                Payload.BudgetLogPreviousSeconds;
            Next.BudgetClock.BudgetScreenshotPreviousSeconds =
                Payload.BudgetScreenshotPreviousSeconds;
            Next.BudgetClock.BudgetScreenshotIndex =
                Payload.BudgetScreenshotIndex;
            float WallDeltaSeconds =
                (State.StatsClock.FrameClockSeconds == double{})
                    ? Settings.InitialDeltaSeconds
                    : Payload.BudgetClockSeconds -
                          State.StatsClock.FrameClockSeconds;
            Next.StatsClock.StatsRefreshElapsedSeconds += WallDeltaSeconds;
            Next.StatsClock.PolyCountRefreshElapsedSeconds += WallDeltaSeconds;

            Next = func::match(
                Payload.PolyCount,
                [&Next, &Settings](const FRuntimePolyCountStats &PolyCount) {
                  FRenderingState Updated = Next;
                  Updated.PolyCache.CachedPolyCount = PolyCount.PolyCount;
                  Updated.PolyCache.CachedPolyCountMilliseconds =
                      PolyCount.MeasurementMilliseconds;
                  Updated.StatsClock.PolyCountRefreshElapsedSeconds =
                      Settings.IntervalResetElapsedSeconds;
                  return Updated;
                },
                [&Next]() { return Next; });

            Next = func::match(
                Payload.Stats,
                [&Next, &Settings](const FRuntimeStatsViewModel &Stats) {
                  FRenderingState Updated = Next;
                  Updated.Presentation.StatsPresentation =
                      ReduceRuntimeStatsPresentationModel(Stats, Settings);
                  Updated.StatsClock.StatsRefreshElapsedSeconds =
                      Settings.IntervalResetElapsedSeconds;
                  return Updated;
                },
                [&Next]() { return Next; });

            return Next;
          })
      .val;
}

} // namespace RenderingReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
