#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"
#include "Features/Systems/Level/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @brief Request for parsing one level-layout JSON object.
 *
 * @signature struct FLevelJsonObjectRequest
 *
 * User story: As a Level adapter author, JSON object parsing flows through one
 * unary payload before the typed seed reaches RTK thunks and reducers.
 */
struct FLevelJsonObjectRequest {
  TSharedPtr<FJsonObject> Object;
};

struct FLevelSectionLeaves {
  TArray<TSharedPtr<FJsonValue>> Blocks;
  TArray<TSharedPtr<FJsonValue>> Labels;
};

struct FLevelLayoutLeaves {
  FLevelSectionLeaves Terrain;
  FLevelSectionLeaves Town;
  FLevelSectionLeaves Mine;
  TArray<TSharedPtr<FJsonValue>> OverlayLabels;
};

/**
 * @brief Request for parsing one authored enum text field.
 *
 * @signature struct FLevelEnumTextRequest
 *
 * User story: As a data author, invalid enum text can report its JSON field
 * while Level systems keep enum parsing below reducers.
 */
struct FLevelEnumTextRequest {
  FString Text;
  FString FieldName;
};

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
