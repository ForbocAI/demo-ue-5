#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Features/Systems/Rendering/Diagnostics/DiagnosticsAdapters.h"
#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"
#include "Features/Systems/Rendering/SystemsRenderingActions.h"
#include "Features/Systems/SystemsSelectors.h"

#include "Engine/Engine.h"
#include "Features/Systems/Rendering/Diagnostics/DiagnosticsSelectors.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/Rendering/Stats/RenderingStatsSelectors.h"
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

void LogRuntimeBudgetSample(
    const FRuntimeStatsViewModel &Stats,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  const FString Message =
      RenderingStatsSelectors::FormatRuntimeStatsBudgetLogMessage(Stats,
                                                                  Settings);
  UE_LOG(LogForbocRuntimeBudget, Display, TEXT("%s"), *Message);
}

void PresentRuntimeStatsDebugMessage(
    const FRuntimeStatsViewModel &Stats,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
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
      const auto &Settings = RuntimeSelectors::SelectUISettings(State).StatsOverlay;

      double BudgetClockSeconds = RenderingAdapters::SelectRuntimeBudgetClockSeconds();
    float WallDeltaSeconds = (RenderState.StatsClock.FrameClockSeconds ==
                              double{})
        ? Settings.InitialDeltaSeconds
        : (BudgetClockSeconds - RenderState.StatsClock.FrameClockSeconds);

    bool bRefreshPolyCount =
        (RenderState.StatsClock.PolyCountRefreshElapsedSeconds +
         WallDeltaSeconds) >= Settings.PolyCountRefreshIntervalSeconds;
    bool bRefreshStats =
        (RenderState.StatsClock.StatsRefreshElapsedSeconds +
         WallDeltaSeconds) >= Settings.StatsRefreshIntervalSeconds;

    FRuntimeStatsSamplePayload Payload;
    Payload.DeltaSeconds = DeltaSeconds;
    Payload.BudgetClockSeconds = BudgetClockSeconds;

    FRuntimePolyCountStats PolyCount = bRefreshPolyCount
        ? RenderingAdapters::SelectRuntimePolyCountStats(World, Settings)
        : FRuntimePolyCountStats{
              RenderState.PolyCache.CachedPolyCount,
              RenderState.PolyCache.CachedPolyCountMilliseconds};
    Payload.PolyCount = bRefreshPolyCount ? func::just(PolyCount) : func::nothing<FRuntimePolyCountStats>();

    Payload.StatsOverlay = Settings;

    Payload.Stats = bRefreshStats
        ? func::just<FRuntimeStatsViewModel>(RenderingAdapters::SelectRuntimeStats(
            World, DeltaSeconds, WallDeltaSeconds, PolyCount.PolyCount,
            PolyCount.MeasurementMilliseconds, Settings))
        : func::nothing<FRuntimeStatsViewModel>();

    bRefreshStats ? PresentRuntimeStatsDebugMessage(Payload.Stats.value, Settings), void() : void();

    bool bShouldLog = bRefreshStats && RenderingSelectors::ShouldRunRuntimeBudgetWallInterval(
                {BudgetClockSeconds,
                 RenderState.BudgetClock.BudgetLogPreviousSeconds,
                 Settings.BudgetLogIntervalSeconds});

    bShouldLog ? LogRuntimeBudgetSample(Payload.Stats.value, Settings), void() : void();
    Payload.BudgetLogPreviousSeconds =
        bShouldLog ? BudgetClockSeconds
                   : RenderState.BudgetClock.BudgetLogPreviousSeconds;

    float BudgetScreenshotIntervalSeconds = RenderingAdapters::SelectRuntimeBudgetScreenshotIntervalSeconds(Settings);
    bool bShouldScreenshot = bRefreshStats && RenderingSelectors::ShouldRunRuntimeBudgetScreenshot(
                {BudgetClockSeconds,
                 RenderState.BudgetClock.BudgetScreenshotPreviousSeconds,
                 BudgetScreenshotIntervalSeconds}, Settings);

    Payload.BudgetScreenshotIndex =
        bShouldScreenshot
            ? RenderState.BudgetClock.BudgetScreenshotIndex +
                  Settings.BudgetScreenshotIndexStep
            : RenderState.BudgetClock.BudgetScreenshotIndex;
    bShouldScreenshot ? RenderingAdapters::RequestRuntimeBudgetScreenshot(Settings, Payload.BudgetScreenshotIndex), void() : void();
      Payload.BudgetScreenshotPreviousSeconds =
          bShouldScreenshot
              ? BudgetClockSeconds
              : RenderState.BudgetClock.BudgetScreenshotPreviousSeconds;

      Dispatch(RenderingActions::RuntimeStatsSampled()(Payload));
      Resolve();
    });
  };
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
