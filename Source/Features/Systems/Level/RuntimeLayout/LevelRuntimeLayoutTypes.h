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

/**
 * @brief Request for choosing the runtime scale parser by scale mode.
 *
 * @signature struct FLevelRuntimeScaleModeSeedRequest
 *
 * User story: As a Level adapter maintainer, mode-specific parsing stays a
 * pure unary transform with no scalar argument list.
 */
struct FLevelRuntimeScaleModeSeedRequest {
  TSharedPtr<FJsonObject> Object;
  ELevelRuntimeScaleMode Mode = ELevelRuntimeScaleMode::Pad;
};

/**
 * @brief Required block fields before enum validation and location parsing.
 *
 * @signature struct FLevelRuntimeBlockFieldsRequest
 *
 * User story: As a runtime adapter author, a block seed is assembled from one
 * payload before dispatching observed level data through RTK actions.
 */
struct FLevelRuntimeBlockFieldsRequest {
  TSharedPtr<FJsonObject> Object;
  func::Maybe<FString> Id = func::nothing<FString>();
  func::Maybe<FString> Name = func::nothing<FString>();
  func::Maybe<FString> AnchorText = func::nothing<FString>();
  func::Maybe<FLevelRuntimeScaleSeed> Scale =
      func::nothing<FLevelRuntimeScaleSeed>();
  func::Maybe<FString> TextureText = func::nothing<FString>();
};

/**
 * @brief Request for completing a block seed with location fields.
 *
 * @signature struct FLevelRuntimeBlockLocationRequest
 *
 * User story: As an ECS system author, world and lot anchored blocks share one
 * seed payload instead of spreading location decisions across views.
 */
struct FLevelRuntimeBlockLocationRequest {
  TSharedPtr<FJsonObject> Object;
  FLevelRuntimeBlockSeed Seed;
};

/**
 * @brief Required label fields before enum validation and height parsing.
 *
 * @signature struct FLevelRuntimeLabelFieldsRequest
 *
 * User story: As a runtime adapter author, labels are parsed into typed seeds
 * before reducers derive spawn state for the single store.
 */
struct FLevelRuntimeLabelFieldsRequest {
  TSharedPtr<FJsonObject> Object;
  func::Maybe<FString> Id = func::nothing<FString>();
  func::Maybe<FString> Text = func::nothing<FString>();
  func::Maybe<FString> AnchorText = func::nothing<FString>();
  func::Maybe<FString> HeightText = func::nothing<FString>();
  func::Maybe<float> EastLots = func::nothing<float>();
  func::Maybe<float> NorthLots = func::nothing<float>();
  func::Maybe<float> WorldSizeScale = func::nothing<float>();
};

/**
 * @brief Request for completing a label seed with height-specific fields.
 *
 * @signature struct FLevelRuntimeLabelHeightRequest
 *
 * User story: As a Level system maintainer, height behavior remains a typed
 * ECS seed concern and never a view decision.
 */
struct FLevelRuntimeLabelHeightRequest {
  TSharedPtr<FJsonObject> Object;
  FLevelRuntimeLabelSeed Seed;
};

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
