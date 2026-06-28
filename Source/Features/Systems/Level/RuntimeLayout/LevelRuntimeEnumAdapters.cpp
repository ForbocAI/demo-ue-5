#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

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

} // namespace

func::Maybe<ELevelRuntimeScaleMode>
ParseScaleMode(const FLevelRuntimeEnumTextRequest &Request) {
  const func::Maybe<ELevelRuntimeScaleMode> Parsed =
      func::multi_match<FString, ELevelRuntimeScaleMode>(
          Request.Text.ToLower(),
          {
              func::when<FString, ELevelRuntimeScaleMode>(
                  func::equals<FString>(TEXT("building")),
                  [](const FString &) {
                    return ELevelRuntimeScaleMode::Building;
                  }),
              func::when<FString, ELevelRuntimeScaleMode>(
                  func::equals<FString>(TEXT("long_feature")),
                  [](const FString &) {
                    return ELevelRuntimeScaleMode::LongFeature;
                  }),
              func::when<FString, ELevelRuntimeScaleMode>(
                  func::equals<FString>(TEXT("pad")),
                  [](const FString &) { return ELevelRuntimeScaleMode::Pad; }),
          });
  return Parsed.hasValue ? Parsed
                         : LogInvalidEnum<ELevelRuntimeScaleMode>(Request);
}

func::Maybe<ELevelRuntimeAnchorMode>
ParseAnchorMode(const FLevelRuntimeEnumTextRequest &Request) {
  const func::Maybe<ELevelRuntimeAnchorMode> Parsed =
      func::multi_match<FString, ELevelRuntimeAnchorMode>(
          Request.Text.ToLower(),
          {
              func::when<FString, ELevelRuntimeAnchorMode>(
                  func::equals<FString>(TEXT("building_lots")),
                  [](const FString &) {
                    return ELevelRuntimeAnchorMode::BuildingLots;
                  }),
              func::when<FString, ELevelRuntimeAnchorMode>(
                  func::equals<FString>(TEXT("feature_lots")),
                  [](const FString &) {
                    return ELevelRuntimeAnchorMode::FeatureLots;
                  }),
              func::when<FString, ELevelRuntimeAnchorMode>(
                  func::equals<FString>(TEXT("post_office_lots")),
                  [](const FString &) {
                    return ELevelRuntimeAnchorMode::PostOfficeLots;
                  }),
              func::when<FString, ELevelRuntimeAnchorMode>(
                  func::equals<FString>(TEXT("world")),
                  [](const FString &) {
                    return ELevelRuntimeAnchorMode::World;
                  }),
          });
  return Parsed.hasValue ? Parsed
                         : LogInvalidEnum<ELevelRuntimeAnchorMode>(Request);
}

func::Maybe<ELevelRuntimeLabelHeightMode>
ParseLabelHeightMode(const FLevelRuntimeEnumTextRequest &Request) {
  const func::Maybe<ELevelRuntimeLabelHeightMode> Parsed =
      func::multi_match<FString, ELevelRuntimeLabelHeightMode>(
          Request.Text.ToLower(),
          {
              func::when<FString, ELevelRuntimeLabelHeightMode>(
                  func::equals<FString>(TEXT("explicit")),
                  [](const FString &) {
                    return ELevelRuntimeLabelHeightMode::Explicit;
                  }),
              func::when<FString, ELevelRuntimeLabelHeightMode>(
                  func::equals<FString>(TEXT("label_for_scale")),
                  [](const FString &) {
                    return ELevelRuntimeLabelHeightMode::LabelForScale;
                  }),
              func::when<FString, ELevelRuntimeLabelHeightMode>(
                  func::equals<FString>(TEXT("above_block")),
                  [](const FString &) {
                    return ELevelRuntimeLabelHeightMode::AboveBlock;
                  }),
          });
  return Parsed.hasValue
             ? Parsed
             : LogInvalidEnum<ELevelRuntimeLabelHeightMode>(Request);
}

func::Maybe<ELevelRetroTexture>
ParseTexture(const FLevelRuntimeEnumTextRequest &Request) {
  const func::Maybe<ELevelRetroTexture> Parsed =
      func::multi_match<FString, ELevelRetroTexture>(
          Request.Text.ToLower(),
          {
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("terrain_ortho")),
                  [](const FString &) {
                    return ELevelRetroTexture::TerrainOrtho;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("building_timber")),
                  [](const FString &) {
                    return ELevelRetroTexture::BuildingTimber;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("road_dust")),
                  [](const FString &) { return ELevelRetroTexture::RoadDust; }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("water_creek")),
                  [](const FString &) {
                    return ELevelRetroTexture::WaterCreek;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("foliage_riparian")),
                  [](const FString &) {
                    return ELevelRetroTexture::FoliageRiparian;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("rock_granite")),
                  [](const FString &) {
                    return ELevelRetroTexture::RockGranite;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("mine_timber")),
                  [](const FString &) {
                    return ELevelRetroTexture::MineTimber;
                  }),
              func::when<FString, ELevelRetroTexture>(
                  func::equals<FString>(TEXT("marker_paint")),
                  [](const FString &) {
                    return ELevelRetroTexture::MarkerPaint;
                  }),
          });
  return Parsed.hasValue ? Parsed : LogInvalidEnum<ELevelRetroTexture>(Request);
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
