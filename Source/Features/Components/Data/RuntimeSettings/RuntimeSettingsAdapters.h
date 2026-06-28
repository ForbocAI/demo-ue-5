#pragma once

#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace RuntimeSettingsAdapters {

/**
 * @brief Reads all runtime settings from the root JSON object.
 * @signature FDemoRuntimeSettings ReadDemoRuntimeSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a runtime factory author, one typed settings payload should be
 * composed below RTK reducers before state initialization begins.
 */
FDemoRuntimeSettings
ReadDemoRuntimeSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Loads all authored UE demo runtime settings from JSON.
 * @signature FDemoRuntimeSettings LoadDemoRuntimeSettings()
 *
 * User story: As a runtime engineer, one store initialization path can load
 * data once and reduce it into the single RTK store before views render.
 */
FDemoRuntimeSettings LoadDemoRuntimeSettings();

} // namespace RuntimeSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
