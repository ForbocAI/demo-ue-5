#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeScaleAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

struct FBuildingScaleSeedPayload {
  float FrontageFeet = 0.0f;
  float DepthFeet = 0.0f;
  float Stories = 0.0f;
};

struct FLongFeatureScaleSeedPayload {
  float WidthFeet = 0.0f;
  float LengthLots = 0.0f;
  float HeightFeet = 0.0f;
};

struct FPadScaleSeedPayload {
  float WidthFeet = 0.0f;
  float DepthFeet = 0.0f;
  float HeightFeet = 0.0f;
};

/**
 * @brief Builds a building scale seed from validated fields.
 *
 * @signature FLevelRuntimeScaleSeed CreateBuildingScaleSeed(const FBuildingScaleSeedPayload &Payload)
 *
 * User story: As a Level adapter maintainer, field aggregation stays in one
 * unary payload before the seed reaches reducer-owned geometry composition.
 */
FLevelRuntimeScaleSeed
CreateBuildingScaleSeed(const FBuildingScaleSeedPayload &Payload) {
  FLevelRuntimeScaleSeed Seed;
  Seed.Mode = ELevelRuntimeScaleMode::Building;
  Seed.FrontageFeet = Payload.FrontageFeet;
  Seed.DepthFeet = Payload.DepthFeet;
  Seed.Stories = Payload.Stories;
  return Seed;
}

/**
 * @brief Builds a long-feature scale seed from validated fields.
 *
 * @signature FLevelRuntimeScaleSeed CreateLongFeatureScaleSeed(const FLongFeatureScaleSeedPayload &Payload)
 *
 * User story: As a Level adapter maintainer, long-feature dimensions are
 * grouped as data before reducers derive world scale.
 */
FLevelRuntimeScaleSeed
CreateLongFeatureScaleSeed(const FLongFeatureScaleSeedPayload &Payload) {
  FLevelRuntimeScaleSeed Seed;
  Seed.Mode = ELevelRuntimeScaleMode::LongFeature;
  Seed.WidthFeet = Payload.WidthFeet;
  Seed.LengthLots = Payload.LengthLots;
  Seed.HeightFeet = Payload.HeightFeet;
  return Seed;
}

/**
 * @brief Builds a pad scale seed from validated fields.
 *
 * @signature FLevelRuntimeScaleSeed CreatePadScaleSeed(const FPadScaleSeedPayload &Payload)
 *
 * User story: As a Level adapter maintainer, pad dimensions are grouped as a
 * typed payload before reducers derive world scale.
 */
FLevelRuntimeScaleSeed CreatePadScaleSeed(const FPadScaleSeedPayload &Payload) {
  FLevelRuntimeScaleSeed Seed;
  Seed.Mode = ELevelRuntimeScaleMode::Pad;
  Seed.WidthFeet = Payload.WidthFeet;
  Seed.DepthFeet = Payload.DepthFeet;
  Seed.HeightFeet = Payload.HeightFeet;
  return Seed;
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
  const func::Maybe<float> FrontageFeet =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("frontage_feet")});
  const func::Maybe<float> DepthFeet =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("depth_feet")});
  const func::Maybe<float> Stories =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("stories")});
  return FrontageFeet.hasValue && DepthFeet.hasValue && Stories.hasValue
             ? func::just(CreateBuildingScaleSeed(
                   {FrontageFeet.value, DepthFeet.value, Stories.value}))
             : func::nothing<FLevelRuntimeScaleSeed>();
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
  const func::Maybe<float> WidthFeet =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("width_feet")});
  const func::Maybe<float> LengthLots =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("length_lots")});
  const func::Maybe<float> HeightFeet =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("height_feet")});
  return WidthFeet.hasValue && LengthLots.hasValue && HeightFeet.hasValue
             ? func::just(CreateLongFeatureScaleSeed(
                   {WidthFeet.value, LengthLots.value, HeightFeet.value}))
             : func::nothing<FLevelRuntimeScaleSeed>();
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
  const func::Maybe<float> WidthFeet =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("width_feet")});
  const func::Maybe<float> DepthFeet =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("depth_feet")});
  const func::Maybe<float> HeightFeet =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("height_feet")});
  return WidthFeet.hasValue && DepthFeet.hasValue && HeightFeet.hasValue
             ? func::just(CreatePadScaleSeed(
                   {WidthFeet.value, DepthFeet.value, HeightFeet.value}))
             : func::nothing<FLevelRuntimeScaleSeed>();
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
  const func::Maybe<float> X =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("x")});
  const func::Maybe<float> Y =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("y")});
  const func::Maybe<float> Z =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("z")});
  return X.hasValue && Y.hasValue && Z.hasValue
             ? func::just(FVector(X.value, Y.value, Z.value))
             : func::nothing<FVector>();
}

func::Maybe<FLevelRuntimeScaleSeed>
ScaleFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredString({Request.Object, TEXT("mode")}),
      [Request](const FString &ModeText) {
        const FLevelRuntimeEnumTextRequest ModeRequest{ModeText,
                                                       TEXT("mode")};
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
