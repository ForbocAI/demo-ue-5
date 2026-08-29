#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Sky/Moon/MoonTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FMoonGeometry, bMoonDiscEnabled,
                       MoonDiscMeshPath, MoonDiscMaterialPath, MoonDiscDistance,
                       MoonDiscScale, RuntimeMoonDiscActorTag);
JSON_SETTINGS_REGISTRY(RenderingProfile::FOrientation,
                       MoonDiscPitchDegrees, MoonDiscYawDegrees,
                       MoonDiscRollDegrees);
JSON_SETTINGS_REGISTRY(RenderingProfile::FMoonColor, MoonDiscColorR,
                       MoonDiscColorG, MoonDiscColorB, MoonDiscColorA);
JSON_SETTINGS_REGISTRY(RenderingProfile::FPixels, MoonPixelGridSize,
                       MoonDiscWorldUnitsPerScale,
                       MoonPixelOverlapMultiplier,
                       MoonPixelGridTerminalOffset, MoonPixelVisibleRadius,
                       RuntimeMoonPixelsComponentName);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FMoon, Geometry, Orientation,
                               Color, Pixels);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
