#pragma once

#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace LevelSettingsAdapters {

/**
 * @brief Reads level terrain source settings from one JSON object.
 * @signature FLevelTerrainSourceSettings ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a terrain thunk author, authored terrain source paths become
 * typed data before the terrain loaded action is dispatched.
 */
FLevelTerrainSourceSettings
ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Reads level geometry settings from one JSON object.
 * @signature FLevelGeometrySettings ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a level reducer author, authored geometry constants are
 * reduced into state once and reused by selectors and ECS projections.
 */
FLevelGeometrySettings
ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object);

} // namespace LevelSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
