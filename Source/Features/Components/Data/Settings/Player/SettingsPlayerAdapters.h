#pragma once

#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace PlayerSettingsAdapters {

/**
 * @fn FPresentationSettings ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads player presentation settings from one JSON object.
 *
 * User story: As a player reducer author, authored player presentation data is
 * converted to a typed payload before it enters the RTK store.
 * User Story: As a data settings player consumer, I need to invoke read player presentation settings through a stable signature so the data settings player workflow remains explicit and composable.
 */
FPresentationSettings
ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn FInteractionSettings ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads interaction settings from one JSON object.
 *
 * User story: As an interaction reducer author, authored interaction values
 * become typed payload data before selectors derive view behavior.
 * User Story: As a data settings player consumer, I need to invoke read interaction settings through a stable signature so the data settings player workflow remains explicit and composable.
 */
FInteractionSettings
ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object);

} // namespace PlayerSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
