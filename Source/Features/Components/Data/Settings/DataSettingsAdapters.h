#pragma once

#include "Core/ecs.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace SettingsAdapters {

/**
 * @fn FSettings ReadSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads all runtime settings from a root manifest JSON object.
 *
 * User story: As a runtime factory author, split subdomain JSON files should
 * compose into one typed settings payload before state initialization begins.
 * User Story: As a components data settings consumer, I need to invoke read settings through a stable signature so the components data settings workflow remains explicit and composable.
 */
FSettings
ReadSettings(const TSharedPtr<FJsonObject> &Object);

/** User Story: As a components data settings consumer, I need to invoke ecs domain registry through a stable signature so the components data settings workflow remains explicit and composable. @fn ecs::FGraph EcsDomainRegistry(const FEcsSettings &Settings) */
ecs::FGraph EcsDomainRegistry(const FEcsSettings &Settings);

/**
 * @fn FSettings LoadSettings()
 * @brief Loads all authored UE runtime runtime settings from JSON.
 *
 * User story: As a runtime engineer, one store initialization path can load
 * data once and reduce it into the single RTK store before views render.
 * User Story: As a components data settings consumer, I need to invoke load settings through a stable signature so the components data settings workflow remains explicit and composable.
 */
FSettings LoadSettings();

} // namespace SettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
