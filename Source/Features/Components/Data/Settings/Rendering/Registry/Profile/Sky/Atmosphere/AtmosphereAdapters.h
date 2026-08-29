#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Sky/Atmosphere/AtmosphereTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FSkyLuminance,
                       SkyAtmosphereSkyLuminanceR,
                       SkyAtmosphereSkyLuminanceG,
                       SkyAtmosphereSkyLuminanceB,
                       SkyAtmosphereSkyLuminanceA);
JSON_SETTINGS_REGISTRY(RenderingProfile::FAerialLuminance,
                       SkyAtmosphereAerialLuminanceR,
                       SkyAtmosphereAerialLuminanceG,
                       SkyAtmosphereAerialLuminanceB,
                       SkyAtmosphereAerialLuminanceA);
JSON_SETTINGS_REGISTRY(RenderingProfile::FContribution,
                       SkyAtmosphereHeightFogContribution);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FAtmosphere, SkyLuminance,
                               AerialLuminance, Contribution);
JSON_SETTINGS_REGISTRY(RenderingProfile::FSkyLight, SkyLightIntensity,
                       SkyLightColorR, SkyLightColorG, SkyLightColorB,
                       SkyLightColorA, bSkyLightRealTimeCapture);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
