#include "Features/Systems/Level/Layout/Enum/EnumAdapters.h"

#include "Features/Components/Data/Json/Value/ValueAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Game::Level;

using ELevelRetroTexture = LevelTypes::ELevelRetroTexture;
using ELevelAnchorMode = LevelTypes::ELevelAnchorMode;
using ELevelLabelHeightMode =
    LevelTypes::ELevelLabelHeightMode;
using ELevelScaleMode = LevelTypes::ELevelScaleMode;

template <> struct TJsonTextValueRegistry<ELevelScaleMode> {
  static const TArray<TTextValueDeclaration<ELevelScaleMode>> &Values() {
    static const TArray<TTextValueDeclaration<ELevelScaleMode>>
        RegisteredValues = {
            {"building", ELevelScaleMode::Building},
            {"long_feature", ELevelScaleMode::LongFeature},
            {"pad", ELevelScaleMode::Pad}};
    return RegisteredValues;
  }
};

template <> struct TJsonTextValueRegistry<ELevelAnchorMode> {
  static const TArray<TTextValueDeclaration<ELevelAnchorMode>>
      &Values() {
    static const TArray<TTextValueDeclaration<ELevelAnchorMode>>
        RegisteredValues = {
            {"building_lots", ELevelAnchorMode::BuildingLots},
            {"feature_lots", ELevelAnchorMode::FeatureLots},
            {"post_office_lots", ELevelAnchorMode::PostOfficeLots},
            {"world", ELevelAnchorMode::World}};
    return RegisteredValues;
  }
};

template <> struct TJsonTextValueRegistry<ELevelLabelHeightMode> {
  static const TArray<TTextValueDeclaration<ELevelLabelHeightMode>>
      &Values() {
    static const TArray<TTextValueDeclaration<ELevelLabelHeightMode>>
        RegisteredValues = {
            {"explicit", ELevelLabelHeightMode::Explicit},
            {"label_for_scale",
             ELevelLabelHeightMode::LabelForScale},
            {"above_block", ELevelLabelHeightMode::AboveBlock}};
    return RegisteredValues;
  }
};

template <> struct TJsonTextValueRegistry<ELevelRetroTexture> {
  static const TArray<TTextValueDeclaration<ELevelRetroTexture>> &Values() {
    static const TArray<TTextValueDeclaration<ELevelRetroTexture>>
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
 * @brief Logs one invalid level-layout enum field.
 *
 * @signature template <typename Output> func::Maybe<Output> LogInvalidEnum(const FLevelEnumTextRequest &Request)
 *
 * User story: As a data adapter author, enum parse failures share the neutral
 * required-field logger while preserving typed Maybe results.
 */
template <typename Output>
func::Maybe<Output> LogInvalidEnum(
    const FLevelEnumTextRequest &Request) {
  ForbocAI::Game::Data::JsonValueAdapters::LogInvalidField(
      {TSharedPtr<FJsonObject>(), Request.FieldName});
  return func::nothing<Output>();
}

/**
 * @brief Parses authored enum text through registered declaration data.
 *
 * @signature template <typename Output> func::Maybe<Output> ParseEnumText(const FLevelEnumTextRequest &Request)
 *
 * User story: As a level-layout adapter maintainer, enum parsers should
 * register authored tokens as data while one reusable runner owns lookup and
 * failure.
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

func::Maybe<ELevelScaleMode>
ParseScaleMode(const FLevelEnumTextRequest &Request) {
  return ParseEnumText<ELevelScaleMode>(Request);
}

func::Maybe<ELevelAnchorMode>
ParseAnchorMode(const FLevelEnumTextRequest &Request) {
  return ParseEnumText<ELevelAnchorMode>(Request);
}

func::Maybe<ELevelLabelHeightMode>
ParseLabelHeightMode(const FLevelEnumTextRequest &Request) {
  return ParseEnumText<ELevelLabelHeightMode>(Request);
}

func::Maybe<ELevelRetroTexture>
ParseTexture(const FLevelEnumTextRequest &Request) {
  return ParseEnumText<ELevelRetroTexture>(Request);
}

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
