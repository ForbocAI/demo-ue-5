#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Features/Systems/Rendering/Diagnostics/DiagnosticsAdapters.h"
#include "Features/Systems/Rendering/Stats/Effects/EffectsAdapters.h"
#include "Features/Systems/Rendering/Stats/Sampling/SamplingAdapters.h"
#include "Features/Systems/Rendering/Stats/Sampling/SamplingSelectors.h"
#include "Features/Systems/Rendering/SystemsRenderingActions.h"
#include "Features/Systems/SystemsSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {

// Rendering thunk implementations are split by subdomain:
// - RenderingProfileThunks.cpp owns runtime profile observation/application.
// - RenderingTextureThunks.cpp owns retro texture/material generation.
// - RenderingDiagnosticsThunks.cpp owns HUD and runtime-budget diagnostics.

/** User Story: As a features systems rendering consumer, I need to invoke observe runtime stats tick through a stable signature so the features systems rendering workflow remains explicit and composable. @fn rtk::ThunkAction<void, FRuntimeState> ObserveRuntimeStatsTick(UWorld *World, float DeltaSeconds) */
rtk::ThunkAction<void, FRuntimeState>
ObserveRuntimeStatsTick(UWorld *World, float DeltaSeconds) {
  return [World, DeltaSeconds](std::function<rtk::AnyAction(const rtk::AnyAction &)> Dispatch,
                 std::function<const FRuntimeState &()> GetState) -> func::AsyncResult<void> {
    return func::createAsyncResult([=](std::function<void()> Resolve, std::function<void(std::string)> Reject) {
      (void)Reject;
      const FRuntimeState &State = GetState();
      const FRenderingState &RenderState = RuntimeSelectors::SelectRenderingState(State);
      const auto &Settings = RuntimeSelectors::SelectUISettings(State).StatsOverlay;
      const double BudgetClockSeconds =
          RenderingAdapters::SelectRuntimeBudgetClockSeconds();
      const float BudgetScreenshotIntervalSeconds =
          RenderingAdapters::SelectRuntimeBudgetScreenshotIntervalSeconds(
              Settings);
      const FRuntimeStatsSamplingPlan Plan =
          RenderingStatsSelectors::SelectRuntimeStatsSamplingPlan(
              {&RenderState, &Settings, BudgetClockSeconds,
               BudgetScreenshotIntervalSeconds});
      const FRuntimeStatsObservation Observation =
          RenderingStatsAdapters::ObserveRuntimeStats(
              {World, &RenderState, &Settings, DeltaSeconds, &Plan.Refresh});
      func::match(
          Observation.Stats,
          [&Plan, &Settings](const FRuntimeStatsViewModel &Stats) {
            check(RenderingStatsAdapters::ExecuteRuntimeStatsEffects(
                {&Plan.Budget.Effects, &Stats, &Settings,
                 Plan.Budget.Screenshot.Index}));
          },
          []() {});
      const FRuntimeStatsSamplePayload Payload =
          RenderingStatsSelectors::SelectRuntimeStatsSamplePayload(
              {DeltaSeconds, &Plan, &Settings, Observation});
      Dispatch(RenderingActions::RuntimeStatsSampled()(Payload));
      Resolve();
    });
  };
}

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
