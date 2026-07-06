#pragma once

#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace RuntimeSettingsAdapters {

/**
 * @brief Reads all runtime settings from a root manifest JSON object.
 * @signature FRuntimeSettings ReadRuntimeSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a runtime factory author, split subdomain JSON files should
 * compose into one typed settings payload before state initialization begins.
 */
FRuntimeSettings
ReadRuntimeSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Loads all authored UE runtime runtime settings from JSON.
 * @signature FRuntimeSettings LoadRuntimeSettings()
 *
 * User story: As a runtime engineer, one store initialization path can load
 * data once and reduce it into the single RTK store before views render.
 */
FRuntimeSettings LoadRuntimeSettings();

} // namespace RuntimeSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
