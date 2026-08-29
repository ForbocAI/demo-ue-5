#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Fog/FogTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FFogState, bFogEnabled,
                       bVolumetricFogEnabled);
JSON_SETTINGS_REGISTRY(RenderingProfile::FShape, FogDensity,
                       FogHeightFalloff, FogStartDistance, FogCutoffDistance,
                       FogMaxOpacity);
JSON_SETTINGS_REGISTRY(RenderingProfile::FFogColor, FogColorR, FogColorG,
                       FogColorB, FogColorA);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FFog, State, Shape, Color);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
