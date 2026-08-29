#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Sky/Dome/DomeTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FDomeGeometry, bSkyDomeEnabled,
                       SkyDomeMeshPath, SkyDomeMaterialPath, SkyDomeScale,
                       SkyDomeZOffset, RuntimeSkyDomeActorTag);
JSON_SETTINGS_REGISTRY(RenderingProfile::FIllumination,
                       SkyDomeSkyBrightness, SkyDomeCloudBrightness,
                       SkyDomeCloudDarkness, SkyDomeRimBrightness);
JSON_SETTINGS_REGISTRY(RenderingProfile::FStarColor, SkyDomeStarColorR,
                       SkyDomeStarColorG, SkyDomeStarColorB,
                       SkyDomeStarColorA);
JSON_SETTINGS_REGISTRY(RenderingProfile::FHorizonColor,
                       SkyDomeHorizonColorR, SkyDomeHorizonColorG,
                       SkyDomeHorizonColorB, SkyDomeHorizonColorA);
JSON_SETTINGS_REGISTRY(RenderingProfile::FZenithColor,
                       SkyDomeZenithColorR, SkyDomeZenithColorG,
                       SkyDomeZenithColorB, SkyDomeZenithColorA);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FSkyDome, Geometry,
                               Illumination, StarColor, HorizonColor,
                               ZenithColor);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
