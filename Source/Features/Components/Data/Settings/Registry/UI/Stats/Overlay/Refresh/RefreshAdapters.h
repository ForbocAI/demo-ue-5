#pragma once

#include "Features/Components/Data/Json/Settings/Registry/SettingsRegistryAdapters.h"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FRefreshSettings, StatsRefreshIntervalSeconds,
                       PolyCountRefreshIntervalSeconds,
                       BudgetLogIntervalSeconds, IntervalResetElapsedSeconds,
                       BudgetScreenshotIntervalSeconds,
                       BudgetScreenshotDisabledIntervalSeconds);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
