#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Lighting/LightingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FSunOrientation, SunPitchDegrees,
                       SunYawDegrees, SunRollDegrees);
JSON_SETTINGS_REGISTRY(RenderingProfile::FDirectionalLightColor,
                       DirectionalLightColorR, DirectionalLightColorG,
                       DirectionalLightColorB, DirectionalLightColorA);
JSON_SETTINGS_REGISTRY(RenderingProfile::FDirectionalLightValues,
                       DirectionalLightIntensity, DirectionalLightSourceAngle);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FDirectionalLight, Values,
                               Color);
JSON_SETTINGS_REGISTRY(RenderingProfile::FShadows, ShadowCascades,
                       ShadowMaxResolution);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FLighting, Sun, Directional,
                               Shadows);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
