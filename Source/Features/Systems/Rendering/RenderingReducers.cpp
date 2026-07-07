#include "Features/Systems/Rendering/RenderingReducers.h"

#include "Core/ue_fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingReducers {

FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings) {
  return {Settings.LevelCubeMeshPath, Settings.BlockoutMaterialPath};
}

FRenderingPayload ReduceRenderingPayload(
    const FRenderingPayloadRequest &Request) {
  return {Request.Id, Request.RuntimeProfile, Request.TextureCatalog,
          Request.DistanceLodStages, Request.RuntimeSettings};
}

FRenderingState ReduceRenderingProfileObserved(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.RuntimeProfile = Action.PayloadValue.RuntimeProfile;
            Next.TextureCatalog = Action.PayloadValue.TextureCatalog;
            Next.DistanceLodStages = Action.PayloadValue.DistanceLodStages;
            Next.RuntimeSettings = Action.PayloadValue.RuntimeSettings;
            Next.bReady = true;
            return Next;
          })
      .val;
}

FRenderingState ReduceTownspersonPresentationRequested(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPresentationRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            return Next;
          })
      .val;
}

FRenderingState ReduceHorsePresentationRequested(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPresentationRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            return Next;
          })
      .val;
}

struct FStatsFormatContext {
  const FString *Format;
  int32 BufferCharacterCount;
};

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

inline FStatsTextModel ReduceRuntimeStatsValue(
    int64 Value, int32 MediumThreshold, int32 HighThreshold, const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
  Model.Color = Value >= HighThreshold ? Settings.HighValueColor
              : (Value >= MediumThreshold ? Settings.MediumValueColor : Settings.LowValueColor);
  return Model;
}

inline FStatsTextModel ReduceRuntimeStatsDecimalValue(
    double Value, const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.DecimalValueFormat, Settings.FormatBufferCharacterCount},
      Value);
  Model.Color = Settings.TextColor;
  return Model;
}

inline FStatsTextModel ReduceRuntimeStatsPlainValue(
    int32 Value, const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
  Model.Color = Settings.TextColor;
  return Model;
}

inline FStatsTextModel ReduceRuntimeStatsPlainValue(
    int64 Value, const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  FStatsTextModel Model;
  Model.Text = FormatRuntimeStatsText(
      {&Settings.ValueFormat, Settings.FormatBufferCharacterCount},
      static_cast<long long>(Value));
  Model.Color = Settings.TextColor;
  return Model;
}

inline FRuntimeStatsPresentationModel ReduceRuntimeStatsPresentationModel(
    const FRuntimeStatsViewModel &Stats,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  FRuntimeStatsPresentationModel Presentation;
  Presentation.FramesPerSecond = ReduceRuntimeStatsValue(Stats.FramesPerSecond, Settings.FramesPerSecondMediumThreshold, Settings.FramesPerSecondHighThreshold, Settings);
  Presentation.StackDepth = ReduceRuntimeStatsValue(Stats.StackDepth, Settings.StackDepthMediumThreshold, Settings.StackDepthHighThreshold, Settings);
  Presentation.PolyCount = ReduceRuntimeStatsValue(Stats.PolyCount, Settings.PolyCountMediumThreshold, Settings.PolyCountHighThreshold, Settings);
  Presentation.UsedPhysicalMemoryMegabytes = ReduceRuntimeStatsValue(Stats.UsedPhysicalMemoryMegabytes, Settings.MemoryMediumThreshold, Settings.MemoryHighThreshold, Settings);
  Presentation.PeakPhysicalMemoryMegabytes = ReduceRuntimeStatsValue(Stats.PeakPhysicalMemoryMegabytes, Settings.MemoryMediumThreshold, Settings.MemoryHighThreshold, Settings);
  Presentation.UsedVirtualMemoryMegabytes = ReduceRuntimeStatsValue(Stats.UsedVirtualMemoryMegabytes, Settings.MemoryMediumThreshold, Settings.MemoryHighThreshold, Settings);
  Presentation.GameThreadMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.GameThreadMilliseconds, Settings);
  Presentation.RenderThreadMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.RenderThreadMilliseconds, Settings);
  Presentation.RhiThreadMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.RhiThreadMilliseconds, Settings);
  Presentation.GpuMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.GpuMilliseconds, Settings);
  Presentation.DrawCalls = ReduceRuntimeStatsPlainValue(Stats.DrawCalls, Settings);
  Presentation.RhiPrimitives = ReduceRuntimeStatsPlainValue(Stats.RhiPrimitives, Settings);
  Presentation.WallDeltaMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.WallDeltaMilliseconds, Settings);
  Presentation.InputDeltaMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.InputDeltaMilliseconds, Settings);
  Presentation.StatsSelectionMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.StatsSelectionMilliseconds, Settings);
  Presentation.PolyCountMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.PolyCountMilliseconds, Settings);
  Presentation.EngineIdleMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.EngineIdleMilliseconds, Settings);
  Presentation.EngineIdleOvershootMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.EngineIdleOvershootMilliseconds, Settings);
  Presentation.MaxFps = ReduceRuntimeStatsDecimalValue(Stats.MaxFps, Settings);
  Presentation.FrameRateLimit = ReduceRuntimeStatsDecimalValue(Stats.FrameRateLimit, Settings);
  Presentation.EffectiveMaxTickRate = ReduceRuntimeStatsDecimalValue(Stats.EffectiveMaxTickRate, Settings);
  Presentation.FixedFrameRateEnabled = ReduceRuntimeStatsPlainValue(Stats.FixedFrameRateEnabled, Settings);
  Presentation.FixedFrameRate = ReduceRuntimeStatsDecimalValue(Stats.FixedFrameRate, Settings);
  Presentation.FixedTimeStepEnabled = ReduceRuntimeStatsPlainValue(Stats.FixedTimeStepEnabled, Settings);
  Presentation.FixedDeltaMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.FixedDeltaMilliseconds, Settings);
  Presentation.VsyncEnabled = ReduceRuntimeStatsPlainValue(Stats.VsyncEnabled, Settings);
  Presentation.IdleWhenNotForegroundEnabled = ReduceRuntimeStatsPlainValue(Stats.IdleWhenNotForegroundEnabled, Settings);
  Presentation.AppHasFocus = ReduceRuntimeStatsPlainValue(Stats.AppHasFocus, Settings);
  Presentation.CpuThrottleEnabled = ReduceRuntimeStatsPlainValue(Stats.CpuThrottleEnabled, Settings);
  Presentation.AllWindowsHidden = ReduceRuntimeStatsPlainValue(Stats.AllWindowsHidden, Settings);
  Presentation.RootReducerMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.RootReducerMilliseconds, Settings);
  Presentation.CombinedReducerMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.CombinedReducerMilliseconds, Settings);
  Presentation.EcsProjectionMilliseconds = ReduceRuntimeStatsDecimalValue(Stats.EcsProjectionMilliseconds, Settings);
  Presentation.ProjectedEntityCount = ReduceRuntimeStatsPlainValue(Stats.ProjectedEntityCount, Settings);
  Presentation.ProjectedComponentTypeCount = ReduceRuntimeStatsPlainValue(Stats.ProjectedComponentTypeCount, Settings);
  return Presentation;
}

FRenderingState ReduceRuntimeStatsSampled(
    const FRenderingState &State,
    const rtk::PayloadAction<FRuntimeStatsSamplePayload> &Action) {
  return (func::pipe(State) |
          [&Action, &State](FRenderingState Next) -> FRenderingState {
            const auto &Payload = Action.PayloadValue;
            const auto &Settings = Payload.StatsOverlay;

            Next.LastFrameClockSeconds = Payload.BudgetClockSeconds;
            Next.BudgetLogLastSeconds = Payload.BudgetLogLastSeconds;
            Next.BudgetScreenshotLastSeconds = Payload.BudgetScreenshotLastSeconds;
            Next.BudgetScreenshotIndex = Payload.BudgetScreenshotIndex;
            float WallDeltaSeconds = (State.LastFrameClockSeconds == 0.0)
                                         ? 0.0f
                                         : Payload.BudgetClockSeconds - State.LastFrameClockSeconds;
            Next.StatsRefreshElapsedSeconds += WallDeltaSeconds;
            Next.PolyCountRefreshElapsedSeconds += WallDeltaSeconds;

            Next = func::match(
                Payload.PolyCount,
                [&Next, &Settings](const FRuntimePolyCountStats &PolyCount) {
                  FRenderingState Updated = Next;
                  Updated.CachedPolyCount = PolyCount.PolyCount;
                  Updated.CachedPolyCountMilliseconds = PolyCount.MeasurementMilliseconds;
                  Updated.PolyCountRefreshElapsedSeconds = Settings.IntervalResetElapsedSeconds;
                  return Updated;
                },
                [&Next]() { return Next; });

            Next = func::match(
                Payload.Stats,
                [&Next, &Settings](const FRuntimeStatsViewModel &Stats) {
                  FRenderingState Updated = Next;
                  Updated.StatsPresentation = ReduceRuntimeStatsPresentationModel(Stats, Settings);

                  Updated.StatsRefreshElapsedSeconds = Settings.IntervalResetElapsedSeconds;
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
