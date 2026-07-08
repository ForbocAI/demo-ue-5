#pragma once

#include "Core/ecs.hpp"
#include "Features/Components/Data/Types.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace SettingsAdapters {

/**
 * @brief Reads all runtime settings from a root manifest JSON object.
 * @signature FSettings ReadSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a runtime factory author, split subdomain JSON files should
 * compose into one typed settings payload before state initialization begins.
 */
FSettings
ReadSettings(const TSharedPtr<FJsonObject> &Object);

ecs::FDomainRegistry EcsDomainRegistry(const FEcsSettings &Settings);

/**
 * @brief Loads all authored UE runtime runtime settings from JSON.
 * @signature FSettings LoadSettings()
 *
 * User story: As a runtime engineer, one store initialization path can load
 * data once and reduce it into the single RTK store before views render.
 */
FSettings LoadSettings();

} // namespace SettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
