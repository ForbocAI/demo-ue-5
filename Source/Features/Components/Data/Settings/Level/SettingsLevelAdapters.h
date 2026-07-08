#pragma once

#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
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
 * @brief Reads authored level data source paths from one JSON object.
 * @signature FLevelDataSourceSettings ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a runtime thunk author, every authored data file path should
 * enter the single RTK store as settings data instead of feature constants.
 */
FLevelDataSourceSettings
ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Reads runtime validation settings from one JSON object.
 * @signature FLevelValidationSettings ReadLevelValidationSettings(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As an automation test author, expected authored data dimensions
 * should live in JSON beside the source data instead of test constants.
 */
FLevelValidationSettings
ReadLevelValidationSettings(const TSharedPtr<FJsonObject> &Object);

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
} // namespace Game
} // namespace ForbocAI
