#pragma once

#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace PlayerSettingsAdapters {

/**
 * @brief Reads player presentation settings from one JSON object.
 * @signature FPlayerPresentationSettings ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a player reducer author, authored player presentation data is
 * converted to a typed payload before it enters the RTK store.
 */
FPlayerPresentationSettings
ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Reads interaction settings from one JSON object.
 * @signature FInteractionSettings ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As an interaction reducer author, authored interaction values
 * become typed payload data before selectors derive view behavior.
 */
FInteractionSettings
ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object);

} // namespace PlayerSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
