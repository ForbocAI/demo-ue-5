#pragma once

#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Sky/Atmosphere/AtmosphereAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Sky/Dome/DomeAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Sky/Moon/MoonAdapters.h"
#include "Features/Components/Data/Settings/Rendering/Registry/Profile/Sky/Stars/StarsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FSky, Atmosphere, Light, Dome,
                               Moon, PointStars);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
