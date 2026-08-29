#pragma once

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/Stats/Sampling/StatsSamplingTypes.h"

class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsAdapters {

struct FRuntimeStatsObservationRequest {
  UWorld *World;
  const FRenderingState *State;
  const ForbocAI::Game::Data::FOverlaySettings *Settings;
  float DeltaSeconds;
  const FRuntimeStatsRefreshPlan *Refresh;
};

/** User Story: As a rendering stats thunk, I need live or retained engine observations selected through registered sample-mode handlers so timing predicates stay in the pure plan. @fn FRuntimeStatsObservation ObserveRuntimeStats(const FRuntimeStatsObservationRequest &Request) */
FRuntimeStatsObservation ObserveRuntimeStats(
    const FRuntimeStatsObservationRequest &Request);

} // namespace RenderingStatsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
