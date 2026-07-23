#pragma once

#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace LevelSettingsAdapters {

/**
 * @fn FSourceSettings ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads level terrain source settings from one JSON object.
 *
 * User story: As a terrain thunk author, authored terrain source paths become
 * typed data before the terrain loaded action is dispatched.
 * User Story: As a data settings level consumer, I need to invoke read level terrain source settings through a stable signature so the data settings level workflow remains explicit and composable.
 */
FSourceSettings
ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object);

/** User Story: As a data settings level consumer, I need to invoke read level csv settings through a stable signature so the data settings level workflow remains explicit and composable. @fn FCsvSettings ReadLevelCsvSettings(const TSharedPtr<FJsonObject> &Object) */
FCsvSettings
ReadLevelCsvSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn FDataSourceSettings ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads authored level data source paths from one JSON object.
 *
 * User story: As a runtime thunk author, every authored data file path should
 * enter the single RTK store as settings data instead of feature constants.
 * User Story: As a data settings level consumer, I need to invoke read level data source settings through a stable signature so the data settings level workflow remains explicit and composable.
 */
FDataSourceSettings
ReadLevelDataSourceSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn FValidationSettings ReadLevelValidationSettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads runtime validation settings from one JSON object.
 *
 * User story: As an automation test author, expected authored data dimensions
 * should live in JSON beside the source data instead of test constants.
 * User Story: As a data settings level consumer, I need to invoke read level validation settings through a stable signature so the data settings level workflow remains explicit and composable.
 */
FValidationSettings
ReadLevelValidationSettings(const TSharedPtr<FJsonObject> &Object);

/**
 * @fn FGeometrySettings ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object)
 * @brief Reads level geometry settings from one JSON object.
 *
 * User story: As a level reducer author, authored geometry constants are
 * reduced into state once and reused by selectors and ECS projections.
 * User Story: As a data settings level consumer, I need to invoke read level geometry settings through a stable signature so the data settings level workflow remains explicit and composable.
 */
FGeometrySettings
ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object);

} // namespace LevelSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
