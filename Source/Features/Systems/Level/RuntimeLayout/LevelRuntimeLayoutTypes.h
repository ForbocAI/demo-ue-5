#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Level/LevelTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {

/**
 * @brief Request for parsing one runtime-layout JSON object.
 *
 * @signature struct FLevelRuntimeJsonObjectRequest
 *
 * User story: As a Level adapter author, JSON object parsing flows through one
 * unary payload before the typed seed reaches RTK thunks and reducers.
 */
struct FLevelRuntimeJsonObjectRequest {
  TSharedPtr<FJsonObject> Object;
};

/**
 * @brief Request for parsing one authored enum text field.
 *
 * @signature struct FLevelRuntimeEnumTextRequest
 *
 * User story: As a data author, invalid enum text can report its JSON field
 * while Level systems keep enum parsing below reducers.
 */
struct FLevelRuntimeEnumTextRequest {
  FString Text;
  FString FieldName;
};

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
