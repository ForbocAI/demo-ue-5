#pragma once

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/Stats/Sampling/StatsSamplingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsAdapters {

struct FRuntimeStatsEffectRequest {
  const TArray<ERuntimeStatsEffect> *Effects;
  const FRuntimeStatsViewModel *Stats;
  const ForbocAI::Game::Data::FOverlaySettings *Settings;
  int32 ScreenshotIndex;
};

/** User Story: As a rendering stats thunk, I need declared effects routed through one registered catalog so Unreal presentation, logging, and screenshots remain outside pure planning. @fn bool ExecuteRuntimeStatsEffects(const FRuntimeStatsEffectRequest &Request) */
bool ExecuteRuntimeStatsEffects(const FRuntimeStatsEffectRequest &Request);

} // namespace RenderingStatsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
