#include "Features/Systems/Rendering/Stats/Sampling/SamplingSelectors.h"

#include "Features/Systems/Rendering/Diagnostics/DiagnosticsSelectors.h"
#include "Features/Systems/Rendering/Stats/RenderingStatsSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsSelectors {
namespace {

struct FRuntimeStatsEffectRegistration {
  ERuntimeStatsEffect Effect;
  bool bEnabled;
};

/** User Story: As a stats scheduler, I need wall time and interval curried into a unary elapsed-time selector so refresh composition stays explicit. @fn std::function<ERuntimeStatsSampleMode(float)> SelectRuntimeStatsSampleMode(float WallDeltaSeconds, float IntervalSeconds) */
std::function<ERuntimeStatsSampleMode(float)> SelectRuntimeStatsSampleMode(
    float WallDeltaSeconds, float IntervalSeconds) {
  return [WallDeltaSeconds, IntervalSeconds](float ElapsedSeconds) {
    return ShouldRunInterval(ElapsedSeconds + WallDeltaSeconds,
                             IntervalSeconds)
               ? ERuntimeStatsSampleMode::Refresh
               : ERuntimeStatsSampleMode::Retain;
  };
}

/** User Story: As a budget scheduler, I need the current clock and trigger curried into a unary checkpoint transition so log and screenshot clocks share one composition. @fn std::function<FRuntimeStatsBudgetCheckpoint(double)> SelectRuntimeStatsBudgetCheckpoint(double CurrentSeconds, bool bTriggered) */
std::function<FRuntimeStatsBudgetCheckpoint(double)>
SelectRuntimeStatsBudgetCheckpoint(double CurrentSeconds, bool bTriggered) {
  return [CurrentSeconds, bTriggered](double PreviousSeconds) {
    return FRuntimeStatsBudgetCheckpoint{
        bTriggered ? CurrentSeconds : PreviousSeconds, bTriggered};
  };
}

/** User Story: As a stats effect router, I need enabled effects selected from grouped registrations so the thunk executes one ordered declaration list. @fn TArray<ERuntimeStatsEffect> SelectRuntimeStatsEffects(const TArray<FRuntimeStatsEffectRegistration> &Registrations) */
TArray<ERuntimeStatsEffect> SelectRuntimeStatsEffects(
    const TArray<FRuntimeStatsEffectRegistration> &Registrations) {
  return func::filter_map_array(
      Registrations,
      [](const FRuntimeStatsEffectRegistration &Registration) {
        return Registration.bEnabled;
      },
      [](const FRuntimeStatsEffectRegistration &Registration) {
        return Registration.Effect;
      });
}

/** User Story: As a stats reducer caller, I need live poly observations represented explicitly so retained cache values are not mistaken for refreshed samples. @fn func::Maybe<FRuntimePolyCountStats> SelectRuntimePolyCountObservation(const FRuntimeStatsPayloadRequest &Request) */
func::Maybe<FRuntimePolyCountStats> SelectRuntimePolyCountObservation(
    const FRuntimeStatsPayloadRequest &Request) {
  return Request.Plan->Refresh.PolyCountMode ==
                 ERuntimeStatsSampleMode::Refresh
             ? func::just(Request.Observation.PolyCount)
             : func::nothing<FRuntimePolyCountStats>();
}

} // namespace

/** User Story: As a rendering stats scheduler, I need a pure sampling plan so refresh, budget, and effect decisions remain testable outside the Unreal IO boundary. @fn FRuntimeStatsSamplingPlan SelectRuntimeStatsSamplingPlan(const FRuntimeStatsSamplingRequest &Request) */
FRuntimeStatsSamplingPlan SelectRuntimeStatsSamplingPlan(
    const FRuntimeStatsSamplingRequest &Request) {
  const FRenderingState &State = *Request.State;
  const auto &Settings = *Request.Settings;
  const float WallDeltaSeconds =
      State.StatsClock.FrameClockSeconds == double{}
          ? Settings.Measurement.Frame.InitialDeltaSeconds
          : Request.BudgetClockSeconds - State.StatsClock.FrameClockSeconds;
  const FRuntimeStatsRefreshPlan Refresh = {
      Request.BudgetClockSeconds,
      WallDeltaSeconds,
      SelectRuntimeStatsSampleMode(
          WallDeltaSeconds,
          Settings.Refresh.PolyCountRefreshIntervalSeconds)(
          State.StatsClock.PolyCountRefreshElapsedSeconds),
      SelectRuntimeStatsSampleMode(
          WallDeltaSeconds, Settings.Refresh.StatsRefreshIntervalSeconds)(
          State.StatsClock.StatsRefreshElapsedSeconds)};
  const bool bRefreshStats =
      Refresh.StatsMode == ERuntimeStatsSampleMode::Refresh;
  const bool bShouldLog =
      bRefreshStats && RenderingSelectors::ShouldRunRuntimeBudgetWallInterval(
                           {Request.BudgetClockSeconds,
                            State.BudgetClock.BudgetLogPreviousSeconds,
                            Settings.Refresh.BudgetLogIntervalSeconds});
  const bool bShouldScreenshot =
      bRefreshStats && RenderingSelectors::ShouldRunRuntimeBudgetScreenshot(
                           {Request.BudgetClockSeconds,
                            State.BudgetClock.BudgetScreenshotPreviousSeconds,
                            Request.BudgetScreenshotIntervalSeconds},
                           Settings);
  const FRuntimeStatsBudgetCheckpoint Log =
      SelectRuntimeStatsBudgetCheckpoint(
          Request.BudgetClockSeconds, bShouldLog)(
          State.BudgetClock.BudgetLogPreviousSeconds);
  const FRuntimeStatsBudgetCheckpoint ScreenshotCheckpoint =
      SelectRuntimeStatsBudgetCheckpoint(
          Request.BudgetClockSeconds, bShouldScreenshot)(
          State.BudgetClock.BudgetScreenshotPreviousSeconds);
  const int32 ScreenshotIndex =
      bShouldScreenshot
          ? State.BudgetClock.BudgetScreenshotIndex +
                Settings.BudgetCapture.Request.BudgetScreenshotIndexStep
          : State.BudgetClock.BudgetScreenshotIndex;
  const TArray<ERuntimeStatsEffect> Effects = SelectRuntimeStatsEffects(
      {{ERuntimeStatsEffect::Present, bRefreshStats},
       {ERuntimeStatsEffect::BudgetLog, bShouldLog},
       {ERuntimeStatsEffect::BudgetScreenshot, bShouldScreenshot}});
  return {Refresh,
          {Log, {ScreenshotCheckpoint, ScreenshotIndex}, Effects}};
}

/** User Story: As a rendering stats reducer caller, I need observations composed into one action payload so the reducer remains the sole owner of clock, cache, and presentation state. @fn FRuntimeStatsSamplePayload SelectRuntimeStatsSamplePayload(const FRuntimeStatsPayloadRequest &Request) */
FRuntimeStatsSamplePayload SelectRuntimeStatsSamplePayload(
    const FRuntimeStatsPayloadRequest &Request) {
  return {Request.DeltaSeconds,
          Request.Plan->Refresh.BudgetClockSeconds,
          Request.Plan->Budget.Log.PreviousSeconds,
          Request.Plan->Budget.Screenshot.Checkpoint.PreviousSeconds,
          Request.Plan->Budget.Screenshot.Index,
          SelectRuntimePolyCountObservation(Request),
          Request.Observation.Stats,
          *Request.Settings};
}

} // namespace RenderingStatsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
