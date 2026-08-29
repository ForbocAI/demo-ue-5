#pragma once

#include "Features/Components/Data/Json/Settings/Registry/SettingsRegistryAdapters.h"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FCaptureRequestSettings,
                       BudgetScreenshotIntervalCommandLineKey,
                       BudgetScreenshotInitialIndex,
                       BudgetScreenshotIndexStep);
JSON_SETTINGS_REGISTRY(FPathSettings, BudgetScreenshotDirectory,
                       BudgetScreenshotFileNameFormat);
JSON_SETTINGS_REGISTRY(FOutputSettings,
                       bBudgetScreenshotCreateDirectoryTree,
                       bBudgetScreenshotShowUI,
                       bBudgetScreenshotAddFilenameSuffix);
JSON_SETTINGS_CONCERN_REGISTRY(FCaptureSettings, Request, Path, Output);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
