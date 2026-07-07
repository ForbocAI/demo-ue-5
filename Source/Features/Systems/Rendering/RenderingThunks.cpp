#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Features/Systems/Rendering/RenderingDiagnosticsAdapters.h"
#include "Features/Systems/Rendering/RenderingSelectors.h"
#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"

#include "Engine/Engine.h"
#include "Features/Systems/Rendering/RenderingDiagnosticsSelectors.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Rendering/RenderingStatsSelectors.h"
#include "Features/Systems/UI/UISelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {

// Rendering thunk implementations are split by subdomain:
// - RenderingProfileThunks.cpp owns runtime profile observation/application.
// - RenderingTextureThunks.cpp owns retro texture/material generation.
// - RenderingDiagnosticsThunks.cpp owns HUD and runtime-budget diagnostics.

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeBudget, Log, All);

void LogRuntimeBudgetSample(const FRuntimeStatsViewModel &Stats) {
  UE_LOG(LogForbocRuntimeBudget, Display,
         TEXT("runtime-budget sample fps=%d stack_depth=%d poly_count=%lld "
              "memory_mib=%lld peak_memory_mib=%lld virtual_memory_mib=%lld "
              "game_ms=%.2f render_ms=%.2f rhi_ms=%.2f gpu_ms=%.2f "
              "draw_calls=%d rhi_primitives=%d wall_ms=%.2f "
              "input_delta_ms=%.2f stats_select_ms=%.2f poly_count_ms=%.2f "
              "engine_idle_ms=%.2f engine_idle_overshoot_ms=%.2f "
              "max_fps=%.2f frame_rate_limit=%.2f effective_max_tick_rate=%.2f "
              "fixed_frame_rate_enabled=%d fixed_frame_rate=%.2f "
              "fixed_time_step_enabled=%d fixed_delta_ms=%.2f "
              "vsync=%d idle_when_not_foreground=%d app_has_focus=%d "
              "cpu_throttle=%d all_windows_hidden=%d "
              "root_reducer_ms=%.2f combined_reducer_ms=%.2f "
              "ecs_projection_ms=%.2f ecs_entities=%d "
              "ecs_component_types=%d"),
         Stats.FramesPerSecond, Stats.StackDepth, Stats.PolyCount,
         Stats.UsedPhysicalMemoryMegabytes,
         Stats.PeakPhysicalMemoryMegabytes, Stats.UsedVirtualMemoryMegabytes,
         Stats.GameThreadMilliseconds, Stats.RenderThreadMilliseconds,
         Stats.RhiThreadMilliseconds, Stats.GpuMilliseconds, Stats.DrawCalls,
         Stats.RhiPrimitives, Stats.WallDeltaMilliseconds,
         Stats.InputDeltaMilliseconds, Stats.StatsSelectionMilliseconds,
         Stats.PolyCountMilliseconds, Stats.EngineIdleMilliseconds,
         Stats.EngineIdleOvershootMilliseconds, Stats.MaxFps,
         Stats.FrameRateLimit, Stats.EffectiveMaxTickRate,
         Stats.FixedFrameRateEnabled, Stats.FixedFrameRate,
         Stats.FixedTimeStepEnabled, Stats.FixedDeltaMilliseconds,
         Stats.VsyncEnabled,
         Stats.IdleWhenNotForegroundEnabled, Stats.AppHasFocus,
         Stats.CpuThrottleEnabled, Stats.AllWindowsHidden,
         Stats.RootReducerMilliseconds, Stats.CombinedReducerMilliseconds,
         Stats.EcsProjectionMilliseconds, Stats.ProjectedEntityCount,
         Stats.ProjectedComponentTypeCount);
}

void PresentRuntimeStatsDebugMessage(
    const FRuntimeStatsViewModel &Stats,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  GEngine ? (GEngine->AddOnScreenDebugMessage(
                 Settings.DebugMessageKey,
                 Settings.DebugMessageDurationSeconds,
                 Settings.TextColor.ToFColor(true),
                 RenderingStatsSelectors::FormatRuntimeStatsDebugMessage(Stats,
                                                                         Settings)),
             void())
          : void();
}

rtk::ThunkAction<void, FRuntimeState>
ObserveRuntimeStatsTick(UWorld *World, float DeltaSeconds) {
  return [World, DeltaSeconds](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
                 std::function<const FRuntimeState &()> GetState) -> func::AsyncResult<void> {
    return func::createAsyncResult([=](std::function<void()> Resolve, std::function<void(std::string)> Reject) {
      const FRuntimeState &State = GetState();
      const FRenderingState &RenderState = RuntimeSelectors::SelectRenderingState(State);
      const auto &Settings = RuntimeSelectors::SelectUIRuntimeSettings(State).StatsOverlay;

      double BudgetClockSeconds = RenderingAdapters::SelectRuntimeBudgetClockSeconds();
    float WallDeltaSeconds = (RenderState.LastFrameClockSeconds == 0.0)
        ? 0.0f
        : (BudgetClockSeconds - RenderState.LastFrameClockSeconds);

    bool bRefreshPolyCount = (RenderState.PolyCountRefreshElapsedSeconds + WallDeltaSeconds) >= Settings.PolyCountRefreshIntervalSeconds;
    bool bRefreshStats = (RenderState.StatsRefreshElapsedSeconds + WallDeltaSeconds) >= Settings.StatsRefreshIntervalSeconds;

    FRuntimeStatsSamplePayload Payload;
    Payload.DeltaSeconds = DeltaSeconds;
    Payload.BudgetClockSeconds = BudgetClockSeconds;

    FRuntimePolyCountStats PolyCount = bRefreshPolyCount
        ? RenderingAdapters::SelectRuntimePolyCountStats(World, Settings)
        : FRuntimePolyCountStats{RenderState.CachedPolyCount, RenderState.CachedPolyCountMilliseconds};
    Payload.PolyCount = bRefreshPolyCount ? func::just(PolyCount) : func::nothing<FRuntimePolyCountStats>();

    Payload.StatsOverlay = Settings;

    Payload.Stats = bRefreshStats
        ? func::just<FRuntimeStatsViewModel>(RenderingAdapters::SelectRuntimeStats(
            World, DeltaSeconds, WallDeltaSeconds, PolyCount.PolyCount,
            PolyCount.MeasurementMilliseconds, Settings))
        : func::nothing<FRuntimeStatsViewModel>();

    bRefreshStats ? PresentRuntimeStatsDebugMessage(Payload.Stats.value, Settings), void() : void();

    bool bShouldLog = bRefreshStats && RenderingSelectors::ShouldRunRuntimeBudgetWallInterval(
                {BudgetClockSeconds, RenderState.BudgetLogLastSeconds,
                 Settings.BudgetLogIntervalSeconds});

    bShouldLog ? LogRuntimeBudgetSample(Payload.Stats.value), void() : void();
    Payload.BudgetLogLastSeconds = bShouldLog ? BudgetClockSeconds : RenderState.BudgetLogLastSeconds;

    float BudgetScreenshotIntervalSeconds = RenderingAdapters::SelectRuntimeBudgetScreenshotIntervalSeconds(Settings);
    bool bShouldScreenshot = bRefreshStats && RenderingSelectors::ShouldRunRuntimeBudgetScreenshot(
                {BudgetClockSeconds, RenderState.BudgetScreenshotLastSeconds,
                 BudgetScreenshotIntervalSeconds}, Settings);

    Payload.BudgetScreenshotIndex = bShouldScreenshot ? RenderState.BudgetScreenshotIndex + Settings.BudgetScreenshotIndexStep : RenderState.BudgetScreenshotIndex;
    bShouldScreenshot ? RenderingAdapters::RequestRuntimeBudgetScreenshot(Settings, Payload.BudgetScreenshotIndex), void() : void();
      Payload.BudgetScreenshotLastSeconds = bShouldScreenshot ? BudgetClockSeconds : RenderState.BudgetScreenshotLastSeconds;

      Dispatch(RenderingActions::RuntimeStatsSampled()(Payload));
      Resolve();
    });
  };
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
