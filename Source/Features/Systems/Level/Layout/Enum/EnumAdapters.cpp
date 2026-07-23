#include "Features/Systems/Level/Layout/Enum/EnumAdapters.h"

#include "Features/Components/Data/Json/Value/JsonValueAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Game::Level;

using ELevelRetroTexture = LevelTypes::ELevelRetroTexture;
using EAnchorMode = LevelTypes::EAnchorMode;
using ELabelHeightMode =
    LevelTypes::ELabelHeightMode;
using EScaleMode = LevelTypes::EScaleMode;

template <> struct TJsonTextValueRegistry<EScaleMode> {
  /** User Story: As a level layout enum consumer, I need to invoke values through a stable signature so the level layout enum workflow remains explicit and composable. @fn static const TArray<TValueDeclaration<EScaleMode>> &Values() */
  static const TArray<TValueDeclaration<EScaleMode>> &Values() {
    static const TArray<TValueDeclaration<EScaleMode>>
        RegisteredValues = {
            {"building", EScaleMode::Building},
            {"long_feature", EScaleMode::LongFeature},
            {"pad", EScaleMode::Pad}};
    return RegisteredValues;
  }
};

template <> struct TJsonTextValueRegistry<EAnchorMode> {
  /** User Story: As a level layout enum consumer, I need to invoke values through a stable signature so the level layout enum workflow remains explicit and composable. @fn static const TArray<TValueDeclaration<EAnchorMode>> &Values() */
  static const TArray<TValueDeclaration<EAnchorMode>>
      &Values() {
    static const TArray<TValueDeclaration<EAnchorMode>>
        RegisteredValues = {
            {"building_lots", EAnchorMode::BuildingLots},
            {"feature_lots", EAnchorMode::FeatureLots},
            {"post_office_lots", EAnchorMode::PostOfficeLots},
            {"world", EAnchorMode::World}};
    return RegisteredValues;
  }
};

template <> struct TJsonTextValueRegistry<ELabelHeightMode> {
  /** User Story: As a level layout enum consumer, I need to invoke values through a stable signature so the level layout enum workflow remains explicit and composable. @fn static const TArray<TValueDeclaration<ELabelHeightMode>> &Values() */
  static const TArray<TValueDeclaration<ELabelHeightMode>>
      &Values() {
    static const TArray<TValueDeclaration<ELabelHeightMode>>
        RegisteredValues = {
            {"explicit", ELabelHeightMode::Explicit},
            {"label_for_scale",
             ELabelHeightMode::LabelForScale},
            {"above_block", ELabelHeightMode::AboveBlock}};
    return RegisteredValues;
  }
};

template <> struct TJsonTextValueRegistry<ELevelRetroTexture> {
  /** User Story: As a level layout enum consumer, I need to invoke values through a stable signature so the level layout enum workflow remains explicit and composable. @fn static const TArray<TValueDeclaration<ELevelRetroTexture>> &Values() */
  static const TArray<TValueDeclaration<ELevelRetroTexture>> &Values() {
    static const TArray<TValueDeclaration<ELevelRetroTexture>>
        RegisteredValues = {
            {"terrain_ortho", ELevelRetroTexture::TerrainOrtho},
            {"building_timber", ELevelRetroTexture::BuildingTimber},
            {"road_dust", ELevelRetroTexture::RoadDust},
            {"water_creek", ELevelRetroTexture::WaterCreek},
            {"foliage_riparian", ELevelRetroTexture::FoliageRiparian},
            {"rock_granite", ELevelRetroTexture::RockGranite},
            {"mine_timber", ELevelRetroTexture::MineTimber},
            {"marker_paint", ELevelRetroTexture::MarkerPaint}};
    return RegisteredValues;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {
namespace {

namespace JsonText = ForbocAI::Game::Data::JsonAdapters;

/**
 * @fn template <typename Output> func::Maybe<Output> LogInvalidEnum( const FLevelEnumTextRequest &Request)
 * @brief Logs one invalid level-layout enum field.
 *
 *
 * User story: As a data adapter author, enum parse failures share the neutral
 * required-field logger while preserving typed Maybe results.
 * User Story: As a level layout enum consumer, I need to invoke log invalid enum through a stable signature so the level layout enum workflow remains explicit and composable.
 */
template <typename Output>
func::Maybe<Output> LogInvalidEnum(
    const FLevelEnumTextRequest &Request) {
  ForbocAI::Game::Data::JsonValueAdapters::LogInvalidField(
      {TSharedPtr<FJsonObject>(), Request.FieldName});
  return func::nothing<Output>();
}

/**
 * @fn template <typename Output> func::Maybe<Output> ParseEnumText(const FLevelEnumTextRequest &Request)
 * @brief Parses authored enum text through registered declaration data.
 *
 *
 * User story: As a level-layout adapter maintainer, enum parsers should
 * register authored tokens as data while one reusable runner owns lookup and
 * failure.
 * User Story: As a level layout enum consumer, I need to invoke parse enum text through a stable signature so the level layout enum workflow remains explicit and composable.
 */
template <typename Output>
func::Maybe<Output>
ParseEnumText(const FLevelEnumTextRequest &Request) {
  return func::match(
      JsonText::ReadRegisteredTextValue<Output>(Request.Text),
      [](Output Value) { return func::just(Value); },
      [&Request]() { return LogInvalidEnum<Output>(Request); });
}

} // namespace

/** User Story: As a level layout enum consumer, I need to invoke parse scale mode through a stable signature so the level layout enum workflow remains explicit and composable. @fn func::Maybe<EScaleMode> ParseScaleMode(const FLevelEnumTextRequest &Request) */
func::Maybe<EScaleMode>
ParseScaleMode(const FLevelEnumTextRequest &Request) {
  return ParseEnumText<EScaleMode>(Request);
}

/** User Story: As a level layout enum consumer, I need to invoke parse anchor mode through a stable signature so the level layout enum workflow remains explicit and composable. @fn func::Maybe<EAnchorMode> ParseAnchorMode(const FLevelEnumTextRequest &Request) */
func::Maybe<EAnchorMode>
ParseAnchorMode(const FLevelEnumTextRequest &Request) {
  return ParseEnumText<EAnchorMode>(Request);
}

/** User Story: As a level layout enum consumer, I need to invoke parse label height mode through a stable signature so the level layout enum workflow remains explicit and composable. @fn func::Maybe<ELabelHeightMode> ParseLabelHeightMode(const FLevelEnumTextRequest &Request) */
func::Maybe<ELabelHeightMode>
ParseLabelHeightMode(const FLevelEnumTextRequest &Request) {
  return ParseEnumText<ELabelHeightMode>(Request);
}

/** User Story: As a level layout enum consumer, I need to invoke parse texture through a stable signature so the level layout enum workflow remains explicit and composable. @fn func::Maybe<ELevelRetroTexture> ParseTexture(const FLevelEnumTextRequest &Request) */
func::Maybe<ELevelRetroTexture>
ParseTexture(const FLevelEnumTextRequest &Request) {
  return ParseEnumText<ELevelRetroTexture>(Request);
}

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
