#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeScaleAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

struct FWorldLocationFields {
  float X = 0.0f;
  float Y = 0.0f;
  float Z = 0.0f;
};

struct FLevelRuntimeScaleModeSeedRequest {
  TSharedPtr<FJsonObject> Object;
  ELevelRuntimeScaleMode Mode = ELevelRuntimeScaleMode::Pad;
};

/**
 * @brief Creates an empty scale seed for a validated scale mode.
 *
 * @signature FLevelRuntimeScaleSeed ScaleSeedWithMode(ELevelRuntimeScaleMode Mode)
 *
 * User story: As a Level adapter maintainer, mode selection stays as one
 * initial declaration value before field declarations populate dimensions.
 */
FLevelRuntimeScaleSeed ScaleSeedWithMode(ELevelRuntimeScaleMode Mode) {
  FLevelRuntimeScaleSeed Seed;
  Seed.Mode = Mode;
  return Seed;
}

/**
 * @brief Builds a vector from validated world-location fields.
 *
 * @signature FVector WorldLocationFromFields(const FWorldLocationFields &Fields)
 *
 * User story: As an ECS seed author, JSON vector fields are grouped before
 * entering typed spawn state.
 */
FVector WorldLocationFromFields(const FWorldLocationFields &Fields) {
  return FVector(Fields.X, Fields.Y, Fields.Z);
}

/**
 * @brief Parses fields for a building scale seed.
 *
 * @signature func::Maybe<FLevelRuntimeScaleSeed> ReadBuildingScaleSeed(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a data author, missing building dimensions stop the authored
 * seed before it enters RTK state flow.
 */
func::Maybe<FLevelRuntimeScaleSeed>
ReadBuildingScaleSeed(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeScaleSeed>(
      Request.Object,
      {JSON_REQUIRED_FIELDS(FLevelRuntimeScaleSeed, FrontageFeet, DepthFeet,
                            Stories)},
      ScaleSeedWithMode(ELevelRuntimeScaleMode::Building));
}

/**
 * @brief Parses fields for a long-feature scale seed.
 *
 * @signature func::Maybe<FLevelRuntimeScaleSeed> ReadLongFeatureScaleSeed(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a data author, long-feature dimensions remain data-driven
 * while incomplete authored records short-circuit before reducer dispatch.
 */
func::Maybe<FLevelRuntimeScaleSeed>
ReadLongFeatureScaleSeed(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeScaleSeed>(
      Request.Object,
      {JSON_REQUIRED_FIELDS(FLevelRuntimeScaleSeed, WidthFeet, LengthLots,
                            HeightFeet)},
      ScaleSeedWithMode(ELevelRuntimeScaleMode::LongFeature));
}

/**
 * @brief Parses fields for a pad scale seed.
 *
 * @signature func::Maybe<FLevelRuntimeScaleSeed> ReadPadScaleSeed(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a data author, pad dimensions are validated before reducers
 * compose runtime geometry.
 */
func::Maybe<FLevelRuntimeScaleSeed>
ReadPadScaleSeed(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeScaleSeed>(
      Request.Object,
      {JSON_REQUIRED_FIELDS(FLevelRuntimeScaleSeed, WidthFeet, DepthFeet,
                            HeightFeet)},
      ScaleSeedWithMode(ELevelRuntimeScaleMode::Pad));
}

/**
 * @brief Dispatches scale parsing by validated scale mode.
 *
 * @signature func::Maybe<FLevelRuntimeScaleSeed> ReadScaleSeedForMode(const FLevelRuntimeScaleModeSeedRequest &Request)
 *
 * User story: As a Level system maintainer, mode-specific parsing uses
 * functional pattern matching without acting as an RTK store substitute.
 */
func::Maybe<FLevelRuntimeScaleSeed>
ReadScaleSeedForMode(const FLevelRuntimeScaleModeSeedRequest &Request) {
  const func::Maybe<func::Maybe<FLevelRuntimeScaleSeed>> Parsed =
      func::multi_match<ELevelRuntimeScaleMode,
                        func::Maybe<FLevelRuntimeScaleSeed>>(
          Request.Mode,
          {
              func::when<ELevelRuntimeScaleMode,
                         func::Maybe<FLevelRuntimeScaleSeed>>(
                  func::equals<ELevelRuntimeScaleMode>(
                      ELevelRuntimeScaleMode::Building),
                  [Request](ELevelRuntimeScaleMode) {
                    return ReadBuildingScaleSeed({Request.Object});
                  }),
              func::when<ELevelRuntimeScaleMode,
                         func::Maybe<FLevelRuntimeScaleSeed>>(
                  func::equals<ELevelRuntimeScaleMode>(
                      ELevelRuntimeScaleMode::LongFeature),
                  [Request](ELevelRuntimeScaleMode) {
                    return ReadLongFeatureScaleSeed({Request.Object});
                  }),
              func::when<ELevelRuntimeScaleMode,
                         func::Maybe<FLevelRuntimeScaleSeed>>(
                  func::equals<ELevelRuntimeScaleMode>(
                      ELevelRuntimeScaleMode::Pad),
                  [Request](ELevelRuntimeScaleMode) {
                    return ReadPadScaleSeed({Request.Object});
                  }),
          });
  return func::match(
      Parsed,
      [](const func::Maybe<FLevelRuntimeScaleSeed> &Seed) { return Seed; },
      []() { return func::nothing<FLevelRuntimeScaleSeed>(); });
}

} // namespace

func::Maybe<FVector>
WorldLocationFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FWorldLocationFields>(
          Request.Object,
          {JSON_REQUIRED_FIELDS(FWorldLocationFields, X, Y, Z)}),
      WorldLocationFromFields);
}

func::Maybe<FLevelRuntimeScaleSeed>
ScaleFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredField<FString>(Request.Object, "Mode"),
      [Request](const FString &ModeText) {
        const FLevelRuntimeEnumTextRequest ModeRequest{
            ModeText, JsonValues::RequiredFieldName("Mode")};
        const func::Maybe<ELevelRuntimeScaleMode> Mode =
            ParseScaleMode(ModeRequest);
        return func::mbind(Mode, [Request](ELevelRuntimeScaleMode ParsedMode) {
          return ReadScaleSeedForMode({Request.Object, ParsedMode});
        });
      });
}

func::Maybe<FLevelRuntimeScaleSeed>
ReadScaleSeed(const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredObject(Request),
      [](const TSharedPtr<FJsonObject> &ScaleObject) {
        return ScaleFromJson({ScaleObject});
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
