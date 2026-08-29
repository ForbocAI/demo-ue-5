#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/PostProcess/PostProcessTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FBlend,
                       PostProcessPriority, PostProcessBlendRadius,
                       PostProcessBlendWeight,
                       PostProcessEnabledBlendWeightThreshold,
                       RuntimePostProcessActorTag);
JSON_SETTINGS_REGISTRY(RenderingProfile::FGrade,
                       PostProcessSaturationMultiplier,
                       PostProcessContrastMultiplier);
JSON_SETTINGS_REGISTRY(RenderingProfile::FGain, PostProcessGainR,
                       PostProcessGainG, PostProcessGainB, PostProcessGainA);
JSON_SETTINGS_REGISTRY(RenderingProfile::FSceneTint,
                       PostProcessSceneTintR, PostProcessSceneTintG,
                       PostProcessSceneTintB, PostProcessSceneTintA);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FPostProcess, Blend, Grade,
                               Gain, SceneTint);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
