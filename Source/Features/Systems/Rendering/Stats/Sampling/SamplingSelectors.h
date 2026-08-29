#pragma once

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/Stats/Sampling/StatsSamplingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsSelectors {

struct FRuntimeStatsSamplingRequest {
  const FRenderingState *State;
  const ForbocAI::Game::Data::FOverlaySettings *Settings;
  double BudgetClockSeconds;
  float BudgetScreenshotIntervalSeconds;
};

struct FRuntimeStatsPayloadRequest {
  float DeltaSeconds;
  const FRuntimeStatsSamplingPlan *Plan;
  const ForbocAI::Game::Data::FOverlaySettings *Settings;
  FRuntimeStatsObservation Observation;
};

/** User Story: As a rendering stats scheduler, I need a pure sampling plan so refresh, budget, and effect decisions remain testable outside the Unreal IO boundary. @fn FRuntimeStatsSamplingPlan SelectRuntimeStatsSamplingPlan(const FRuntimeStatsSamplingRequest &Request) */
FRuntimeStatsSamplingPlan SelectRuntimeStatsSamplingPlan(
    const FRuntimeStatsSamplingRequest &Request);

/** User Story: As a rendering stats reducer caller, I need observations composed into one action payload so the reducer remains the sole owner of clock, cache, and presentation state. @fn FRuntimeStatsSamplePayload SelectRuntimeStatsSamplePayload(const FRuntimeStatsPayloadRequest &Request) */
FRuntimeStatsSamplePayload SelectRuntimeStatsSamplePayload(
    const FRuntimeStatsPayloadRequest &Request);

} // namespace RenderingStatsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
