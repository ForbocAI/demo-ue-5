#pragma once

#include "Features/Components/Data/Settings/Rendering/Profile/SettingsRenderingProfileTypes.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Fog/FogAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Lighting/LightingAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Material/MaterialAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Output/OutputAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Pixel/PixelAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/PostProcess/PostProcessAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Scalability/ScalabilityAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Sky/SkyAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Time/TimeAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_CONCERN_REGISTRY(FProfileSettings, Time, Output, Scalability,
                               Lighting, PostProcess, Sky, Material, PixelQuad,
                               Fog);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
