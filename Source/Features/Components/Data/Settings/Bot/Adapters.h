#pragma once

#include "Features/Components/Data/Types.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace BotSettingsAdapters {

/**
 * @brief Reads default townsperson settings from one JSON object.
 * @signature FTownspersonDefaultsSettings ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a townsperson reducer author, default NPC payload data is
 * typed before actions enter the single RTK store.
 */
FTownspersonDefaultsSettings
ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Reads townsperson presentation settings from one JSON object.
 * @signature FTownspersonPresentationSettings ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a rendering reducer author, NPC presentation values are
 * prepared below views and reduced into store-owned display state.
 */
FTownspersonPresentationSettings
ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Reads horse presentation settings from one JSON object.
 * @signature FHorsePresentationSettings ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a horse rendering reducer author, authored presentation
 * values become typed payloads before selectors derive actor view models.
 */
FHorsePresentationSettings
ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object);

} // namespace BotSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
