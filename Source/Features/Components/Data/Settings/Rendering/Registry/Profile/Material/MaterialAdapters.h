#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Material/MaterialTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FBaseMaterialParameters,
                       MaterialBaseColorParameter, MaterialColorParameter,
                       MaterialTintColorParameter,
                       MaterialDiffuseColorParameter,
                       MaterialEmissiveColorParameter);
JSON_SETTINGS_REGISTRY(RenderingProfile::FSkyDomeIlluminationParameters,
                       SkyDomeSkyBrightnessParameter,
                       SkyDomeCloudBrightnessParameter,
                       SkyDomeCloudDarknessParameter,
                       SkyDomeRimBrightnessParameter, SkyDomeStarsParameter);
JSON_SETTINGS_REGISTRY(RenderingProfile::FSkyDomeColorParameters,
                       SkyDomeStarColorParameter,
                       SkyDomeHorizonColorParameter,
                       SkyDomeZenithColorParameter);
JSON_SETTINGS_REGISTRY(RenderingProfile::FSkyDomeStarVisibility,
                       SkyDomeStarsScalarValue);
JSON_SETTINGS_REGISTRY(RenderingProfile::FSkyDomeStarMaskColor,
                       SkyDomeTextureStarMaskColorR,
                       SkyDomeTextureStarMaskColorG,
                       SkyDomeTextureStarMaskColorB,
                       SkyDomeTextureStarMaskColorA);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FSkyDomeStars, Visibility,
                               MaskColor);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FMaterial, Base, Illumination,
                               Color, Stars);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
