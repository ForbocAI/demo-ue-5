#pragma once

#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace BotAdapters {

/**
 * @fn FTownspersonDefaultsSettings ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads default townsperson settings from one JSON object.
 *
 * User story: As a townsperson reducer author, default NPC payload data is
 * typed before actions enter the single RTK store.
 * User Story: As a data settings bot consumer, I need to invoke read townsperson defaults settings through a stable signature so the data settings bot workflow remains explicit and composable.
 */
FTownspersonDefaultsSettings
ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn FTownspersonPresentationSettings ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads townsperson presentation settings from one JSON object.
 *
 * User story: As a rendering reducer author, NPC presentation values are
 * prepared below views and reduced into store-owned display state.
 * User Story: As a data settings bot consumer, I need to invoke read townsperson presentation settings through a stable signature so the data settings bot workflow remains explicit and composable.
 */
FTownspersonPresentationSettings
ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn FHorsePresentationSettings ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads horse presentation settings from one JSON object.
 *
 * User story: As a horse rendering reducer author, authored presentation
 * values become typed payloads before selectors derive actor view models.
 * User Story: As a data settings bot consumer, I need to invoke read horse presentation settings through a stable signature so the data settings bot workflow remains explicit and composable.
 */
FHorsePresentationSettings
ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object);

} // namespace BotAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
