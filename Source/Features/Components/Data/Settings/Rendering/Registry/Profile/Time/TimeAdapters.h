#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Time/TimeTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FTime, TimeOfDayHour);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
