#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"

#include <initializer_list>

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

template <typename Output> struct TEnumTextDeclaration {
  FString Text;
  Output Value;

  TEnumTextDeclaration() : Text(), Value() {}

  TEnumTextDeclaration(const char *InText, Output InValue)
      : Text(FString(UTF8_TO_TCHAR(InText))), Value(InValue) {}
};

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
 * @brief Parses lower-case authored enum text through declaration data.
 *
 * @signature template <typename Output> func::Maybe<Output> ParseEnumText(const FLevelRuntimeEnumTextRequest &Request, std::initializer_list<TEnumTextDeclaration<Output>> Declarations)
 *
 * User story: As a runtime-layout adapter maintainer, enum parsers should list
 * authored tokens as data while one reusable runner owns lookup and failure.
 */
template <typename Output>
func::Maybe<Output> ParseEnumText(
    const FLevelRuntimeEnumTextRequest &Request,
    std::initializer_list<TEnumTextDeclaration<Output>> Declarations) {
  const FString Text = Request.Text.ToLower();
  return func::match(
      func::find_array<TEnumTextDeclaration<Output>>(
          TArray<TEnumTextDeclaration<Output>>(Declarations),
          [Text](const TEnumTextDeclaration<Output> &Declaration) {
            return Declaration.Text == Text;
          }),
      [](const TEnumTextDeclaration<Output> &Declaration) {
        return func::just(Declaration.Value);
      },
      [Request]() { return LogInvalidEnum<Output>(Request); });
}

} // namespace

func::Maybe<ELevelRuntimeScaleMode>
ParseScaleMode(const FLevelRuntimeEnumTextRequest &Request) {
  return ParseEnumText<ELevelRuntimeScaleMode>(
      Request, {{"building", ELevelRuntimeScaleMode::Building},
                {"long_feature", ELevelRuntimeScaleMode::LongFeature},
                {"pad", ELevelRuntimeScaleMode::Pad}});
}

func::Maybe<ELevelRuntimeAnchorMode>
ParseAnchorMode(const FLevelRuntimeEnumTextRequest &Request) {
  return ParseEnumText<ELevelRuntimeAnchorMode>(
      Request, {{"building_lots", ELevelRuntimeAnchorMode::BuildingLots},
                {"feature_lots", ELevelRuntimeAnchorMode::FeatureLots},
                {"post_office_lots", ELevelRuntimeAnchorMode::PostOfficeLots},
                {"world", ELevelRuntimeAnchorMode::World}});
}

func::Maybe<ELevelRuntimeLabelHeightMode>
ParseLabelHeightMode(const FLevelRuntimeEnumTextRequest &Request) {
  return ParseEnumText<ELevelRuntimeLabelHeightMode>(
      Request, {{"explicit", ELevelRuntimeLabelHeightMode::Explicit},
                {"label_for_scale",
                 ELevelRuntimeLabelHeightMode::LabelForScale},
                {"above_block", ELevelRuntimeLabelHeightMode::AboveBlock}});
}

func::Maybe<ELevelRetroTexture>
ParseTexture(const FLevelRuntimeEnumTextRequest &Request) {
  return ParseEnumText<ELevelRetroTexture>(
      Request, {{"terrain_ortho", ELevelRetroTexture::TerrainOrtho},
                {"building_timber", ELevelRetroTexture::BuildingTimber},
                {"road_dust", ELevelRetroTexture::RoadDust},
                {"water_creek", ELevelRetroTexture::WaterCreek},
                {"foliage_riparian", ELevelRetroTexture::FoliageRiparian},
                {"rock_granite", ELevelRetroTexture::RockGranite},
                {"mine_timber", ELevelRetroTexture::MineTimber},
                {"marker_paint", ELevelRetroTexture::MarkerPaint}});
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
