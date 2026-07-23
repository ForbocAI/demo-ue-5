#include "Features/Systems/Rendering/SystemsRenderingSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingReducers {

struct FFormatContext {
  const FString *Format;
  int32 BufferCharacterCount;
};

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

/** User Story: As a systems rendering stats consumer, I need to invoke reduce runtime stats value through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FStatsTextModel ReduceRuntimeStatsValue( int64 Value, int32 MediumThreshold, int32 HighThreshold, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
inline FStatsTextModel ReduceRuntimeStatsValue(
    int64 Value, int32 MediumThreshold, int32 HighThreshold,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
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

/** User Story: As a systems rendering stats consumer, I need to invoke reduce runtime stats decimal value through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FStatsTextModel ReduceRuntimeStatsDecimalValue( double Value, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
inline FStatsTextModel ReduceRuntimeStatsDecimalValue(
    double Value,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.DecimalValueFormat, Settings.FormatBufferCharacterCount},
      Value);
  Model.Color = Settings.TextColor;
  return Model;
}

/** User Story: As a systems rendering stats consumer, I need to invoke reduce runtime stats plain value through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FStatsTextModel ReduceRuntimeStatsPlainValue( int32 Value, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
inline FStatsTextModel ReduceRuntimeStatsPlainValue(
    int32 Value,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
  Model.Color = Settings.TextColor;
  return Model;
}

/** User Story: As a systems rendering stats consumer, I need to invoke reduce runtime stats plain value through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FStatsTextModel ReduceRuntimeStatsPlainValue( int64 Value, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
inline FStatsTextModel ReduceRuntimeStatsPlainValue(
    int64 Value,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
  Model.Color = Settings.TextColor;
  return Model;
}

/** User Story: As a systems rendering stats consumer, I need to invoke reduce runtime stats presentation model through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn inline FRuntimeStatsPresentationModel ReduceRuntimeStatsPresentationModel( const FRuntimeStatsViewModel &Stats, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
inline FRuntimeStatsPresentationModel ReduceRuntimeStatsPresentationModel(
    const FRuntimeStatsViewModel &Stats,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  FRuntimeStatsPresentationModel Presentation;
  Presentation.FramesPerSecond = ReduceRuntimeStatsValue(
      Stats.Summary.FramesPerSecond, Settings.FramesPerSecondMediumThreshold,
      Settings.FramesPerSecondHighThreshold, Settings);
  Presentation.StackDepth = ReduceRuntimeStatsValue(
      Stats.Summary.StackDepth, Settings.StackDepthMediumThreshold,
      Settings.StackDepthHighThreshold, Settings);
  Presentation.PolyCount =
      ReduceRuntimeStatsValue(Stats.Summary.PolyCount,
                              Settings.PolyCountMediumThreshold,
                              Settings.PolyCountHighThreshold, Settings);
  Presentation.UsedPhysicalMemoryMegabytes = ReduceRuntimeStatsValue(
      Stats.Memory.UsedPhysicalMegabytes, Settings.MemoryMediumThreshold,
      Settings.MemoryHighThreshold, Settings);
  Presentation.PeakPhysicalMemoryMegabytes = ReduceRuntimeStatsValue(
      Stats.Memory.PeakUsedPhysicalMegabytes, Settings.MemoryMediumThreshold,
      Settings.MemoryHighThreshold, Settings);
  Presentation.UsedVirtualMemoryMegabytes = ReduceRuntimeStatsValue(
      Stats.Memory.UsedVirtualMegabytes, Settings.MemoryMediumThreshold,
      Settings.MemoryHighThreshold, Settings);
  Presentation.GameThreadMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Timing.GameThreadMilliseconds, Settings);
  Presentation.RenderThreadMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Timing.RenderThreadMilliseconds, Settings);
  Presentation.RhiThreadMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Timing.RhiThreadMilliseconds, Settings);
  Presentation.GpuMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Timing.GpuMilliseconds, Settings);
  Presentation.DrawCalls =
      ReduceRuntimeStatsPlainValue(Stats.Timing.DrawCalls, Settings);
  Presentation.RhiPrimitives =
      ReduceRuntimeStatsPlainValue(Stats.Timing.RhiPrimitives, Settings);
  Presentation.WallDeltaMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Delta.WallDeltaMilliseconds, Settings);
  Presentation.InputDeltaMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Delta.InputDeltaMilliseconds, Settings);
  Presentation.StatsSelectionMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Delta.StatsSelectionMilliseconds, Settings);
  Presentation.PolyCountMilliseconds =
      ReduceRuntimeStatsDecimalValue(
          Stats.Pacing.Delta.PolyCountMilliseconds, Settings);
  Presentation.EngineIdleMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Delta.EngineIdleMilliseconds, Settings);
  Presentation.EngineIdleOvershootMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Delta.EngineIdleOvershootMilliseconds,
                                     Settings);
  Presentation.MaxFps =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Rate.MaxFps, Settings);
  Presentation.FrameRateLimit =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Rate.FrameRateLimit, Settings);
  Presentation.EffectiveMaxTickRate =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Rate.EffectiveMaxTickRate, Settings);
  Presentation.FixedFrameRateEnabled =
      ReduceRuntimeStatsPlainValue(
          Stats.Pacing.Mode.FixedFrameRateEnabled, Settings);
  Presentation.FixedFrameRate =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Rate.FixedFrameRate, Settings);
  Presentation.FixedTimeStepEnabled =
      ReduceRuntimeStatsPlainValue(Stats.Pacing.Mode.FixedTimeStepEnabled, Settings);
  Presentation.FixedDeltaMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Pacing.Mode.FixedDeltaMilliseconds, Settings);
  Presentation.VsyncEnabled =
      ReduceRuntimeStatsPlainValue(Stats.Pacing.Mode.VsyncEnabled, Settings);
  Presentation.IdleWhenNotForegroundEnabled =
      ReduceRuntimeStatsPlainValue(Stats.Pacing.Mode.IdleWhenNotForegroundEnabled, Settings);
  Presentation.AppHasFocus =
      ReduceRuntimeStatsPlainValue(Stats.Pacing.Mode.AppHasFocus, Settings);
  Presentation.CpuThrottleEnabled =
      ReduceRuntimeStatsPlainValue(Stats.Pacing.Throttle.CpuThrottleEnabled, Settings);
  Presentation.AllWindowsHidden =
      ReduceRuntimeStatsPlainValue(Stats.Pacing.Throttle.AllWindowsHidden, Settings);
  Presentation.RootReducerMilliseconds =
      ReduceRuntimeStatsDecimalValue(Stats.Reducers.RootReducerMilliseconds, Settings);
  Presentation.CombinedReducerMilliseconds = ReduceRuntimeStatsDecimalValue(
      Stats.Reducers.CombinedReducerMilliseconds, Settings);
  Presentation.EcsProjectionMilliseconds = ReduceRuntimeStatsDecimalValue(
      Stats.Projection.EcsProjectionMilliseconds, Settings);
  Presentation.ProjectedEntityCount =
      ReduceRuntimeStatsPlainValue(Stats.Projection.ProjectedEntityCount, Settings);
  Presentation.ProjectedComponentTypeCount =
      ReduceRuntimeStatsPlainValue(Stats.Projection.ProjectedComponentTypeCount, Settings);
  return Presentation;
}

/** User Story: As a systems rendering stats consumer, I need to invoke reduce runtime stats sampled through a stable signature so the systems rendering stats workflow remains explicit and composable. @fn FRenderingState ReduceRuntimeStatsSampled( const FRenderingState &State, const rtk::PayloadAction<FRuntimeStatsSamplePayload> &Action) */
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
