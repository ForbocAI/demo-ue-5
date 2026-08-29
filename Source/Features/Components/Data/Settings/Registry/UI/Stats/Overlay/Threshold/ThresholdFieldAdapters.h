#pragma once

#include "Features/Components/Data/Json/Settings/Registry/SettingsRegistryAdapters.h"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FPerformanceThresholdSettings,
                       FramesPerSecondMediumThreshold,
                       FramesPerSecondHighThreshold,
                       StackDepthMediumThreshold, StackDepthHighThreshold);
JSON_SETTINGS_REGISTRY(FResourceThresholdSettings, PolyCountMediumThreshold,
                       PolyCountHighThreshold, MemoryMediumThreshold,
                       MemoryHighThreshold);
JSON_SETTINGS_CONCERN_REGISTRY(FThresholdSettings, Performance, Resource);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
