#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Demo::Level;

using ELevelRetroTexture = LevelTypes::ELevelRetroTexture;
using ELevelRuntimeAnchorMode = LevelTypes::ELevelRuntimeAnchorMode;
using ELevelRuntimeLabelHeightMode =
    LevelTypes::ELevelRuntimeLabelHeightMode;
using ELevelRuntimeScaleMode = LevelTypes::ELevelRuntimeScaleMode;

template <> struct TJsonTextValueRegistry<ELevelRuntimeScaleMode> {
  static const TArray<TTextValueDeclaration<ELevelRuntimeScaleMode>> &Values() {
    static const TArray<TTextValueDeclaration<ELevelRuntimeScaleMode>>
        RegisteredValues = {
            {"building", ELevelRuntimeScaleMode::Building},
            {"long_feature", ELevelRuntimeScaleMode::LongFeature},
            {"pad", ELevelRuntimeScaleMode::Pad}};
    return RegisteredValues;
  }
};

template <> struct TJsonTextValueRegistry<ELevelRuntimeAnchorMode> {
  static const TArray<TTextValueDeclaration<ELevelRuntimeAnchorMode>>
      &Values() {
    static const TArray<TTextValueDeclaration<ELevelRuntimeAnchorMode>>
        RegisteredValues = {
            {"building_lots", ELevelRuntimeAnchorMode::BuildingLots},
            {"feature_lots", ELevelRuntimeAnchorMode::FeatureLots},
            {"post_office_lots", ELevelRuntimeAnchorMode::PostOfficeLots},
            {"world", ELevelRuntimeAnchorMode::World}};
    return RegisteredValues;
  }
};

template <> struct TJsonTextValueRegistry<ELevelRuntimeLabelHeightMode> {
  static const TArray<TTextValueDeclaration<ELevelRuntimeLabelHeightMode>>
      &Values() {
    static const TArray<TTextValueDeclaration<ELevelRuntimeLabelHeightMode>>
        RegisteredValues = {
            {"explicit", ELevelRuntimeLabelHeightMode::Explicit},
            {"label_for_scale",
             ELevelRuntimeLabelHeightMode::LabelForScale},
            {"above_block", ELevelRuntimeLabelHeightMode::AboveBlock}};
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
} // namespace Demo
} // namespace ForbocAI

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonText = ForbocAI::Demo::Data::JsonAdapters;

/**
 * @brief Logs one invalid runtime-layout enum field.
 *
 * @signature template <typename Output> func::Maybe<Output> LogInvalidEnum(const FLevelRuntimeEnumTextRequest &Request)
 *
 * User story: As a data adapter author, enum parse failures share the neutral
 * required-field logger while preserving typed Maybe results.
 */
template <typename Output>
func::Maybe<Output> LogInvalidEnum(
    const FLevelRuntimeEnumTextRequest &Request) {
  ForbocAI::Demo::Data::JsonValueAdapters::LogInvalidField(
      {TSharedPtr<FJsonObject>(), Request.FieldName});
  return func::nothing<Output>();
}

/**
 * @brief Parses authored enum text through registered declaration data.
 *
 * @signature template <typename Output> func::Maybe<Output> ParseEnumText(const FLevelRuntimeEnumTextRequest &Request)
 *
 * User story: As a runtime-layout adapter maintainer, enum parsers should
 * register authored tokens as data while one reusable runner owns lookup and
 * failure.
 */
template <typename Output>
func::Maybe<Output>
ParseEnumText(const FLevelRuntimeEnumTextRequest &Request) {
  return func::match(
      JsonText::ReadRegisteredTextValue<Output>(Request.Text),
      [](Output Value) { return func::just(Value); },
      [&Request]() { return LogInvalidEnum<Output>(Request); });
}

} // namespace

func::Maybe<ELevelRuntimeScaleMode>
ParseScaleMode(const FLevelRuntimeEnumTextRequest &Request) {
  return ParseEnumText<ELevelRuntimeScaleMode>(Request);
}

func::Maybe<ELevelRuntimeAnchorMode>
ParseAnchorMode(const FLevelRuntimeEnumTextRequest &Request) {
  return ParseEnumText<ELevelRuntimeAnchorMode>(Request);
}

func::Maybe<ELevelRuntimeLabelHeightMode>
ParseLabelHeightMode(const FLevelRuntimeEnumTextRequest &Request) {
  return ParseEnumText<ELevelRuntimeLabelHeightMode>(Request);
}

func::Maybe<ELevelRetroTexture>
ParseTexture(const FLevelRuntimeEnumTextRequest &Request) {
  return ParseEnumText<ELevelRetroTexture>(Request);
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
