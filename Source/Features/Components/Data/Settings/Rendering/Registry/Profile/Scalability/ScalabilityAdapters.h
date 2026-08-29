#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Scalability/ScalabilityTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FScalability, ViewDistanceScale,
                       FoliageDensityScale, GrassDensityScale);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
