#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLabelAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeScaleAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

struct FLevelRuntimeLabelSeedPayload {
  FString Id;
  FString Text;
  ELevelRuntimeAnchorMode Anchor = ELevelRuntimeAnchorMode::PostOfficeLots;
  ELevelRuntimeLabelHeightMode Height =
      ELevelRuntimeLabelHeightMode::Explicit;
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
  float WorldSizeScale = 1.0f;
};

/**
 * @brief Creates a label seed before height-specific fields are applied.
 *
 * @signature FLevelRuntimeLabelSeed CreateLabelSeed(const FLevelRuntimeLabelSeedPayload &Payload)
 *
 * User story: As a Level adapter maintainer, validated label identity, anchor,
 * location, and size data are grouped before reducer-owned spawn composition.
 */
FLevelRuntimeLabelSeed
CreateLabelSeed(const FLevelRuntimeLabelSeedPayload &Payload) {
  FLevelRuntimeLabelSeed Seed;
  Seed.Id = Payload.Id;
  Seed.Text = Payload.Text;
  Seed.Anchor = Payload.Anchor;
  Seed.Height = Payload.Height;
  Seed.EastLots = Payload.EastLots;
  Seed.NorthLots = Payload.NorthLots;
  Seed.WorldSizeScale = Payload.WorldSizeScale;
  return Seed;
}

/**
 * @brief Applies an explicit height offset to a label seed.
 *
 * @signature func::Maybe<FLevelRuntimeLabelSeed> ReadExplicitHeightLabelSeed(const FLevelRuntimeLabelHeightRequest &Request)
 *
 * User story: As a data author, explicit label height remains authored JSON
 * data and reaches views only after RTK reducers derive placement.
 */
func::Maybe<FLevelRuntimeLabelSeed>
ReadExplicitHeightLabelSeed(const FLevelRuntimeLabelHeightRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("height_offset")}),
      [Request](float HeightOffset) {
        FLevelRuntimeLabelSeed Seed = Request.Seed;
        Seed.HeightOffset = HeightOffset;
        return func::just(Seed);
      });
}

/**
 * @brief Applies a reference scale to a label seed.
 *
 * @signature func::Maybe<FLevelRuntimeLabelSeed> ReadReferenceScaleLabelSeed(const FLevelRuntimeLabelHeightRequest &Request)
 *
 * User story: As a Level reducer author, label height can later derive from
 * typed scale data instead of raw JSON fields.
 */
func::Maybe<FLevelRuntimeLabelSeed>
ReadReferenceScaleLabelSeed(const FLevelRuntimeLabelHeightRequest &Request) {
  return func::mbind(
      ReadScaleSeed({Request.Object, TEXT("reference_scale")}),
      [Request](const FLevelRuntimeScaleSeed &ReferenceScale) {
        FLevelRuntimeLabelSeed Seed = Request.Seed;
        Seed.ReferenceScale = ReferenceScale;
        return func::just(Seed);
      });
}

/**
 * @brief Completes a label seed according to validated height mode.
 *
 * @signature func::Maybe<FLevelRuntimeLabelSeed> CompleteLabelHeight(const FLevelRuntimeLabelHeightRequest &Request)
 *
 * User story: As an ECS system maintainer, label height branching stays in the
 * Level runtime-layout adapter instead of view display code.
 */
func::Maybe<FLevelRuntimeLabelSeed>
CompleteLabelHeight(const FLevelRuntimeLabelHeightRequest &Request) {
  const func::Maybe<func::Maybe<FLevelRuntimeLabelSeed>> Parsed =
      func::multi_match<ELevelRuntimeLabelHeightMode,
                        func::Maybe<FLevelRuntimeLabelSeed>>(
          Request.Seed.Height,
          {
              func::when<ELevelRuntimeLabelHeightMode,
                         func::Maybe<FLevelRuntimeLabelSeed>>(
                  func::equals<ELevelRuntimeLabelHeightMode>(
                      ELevelRuntimeLabelHeightMode::Explicit),
                  [Request](ELevelRuntimeLabelHeightMode) {
                    return ReadExplicitHeightLabelSeed(Request);
                  }),
              func::when<ELevelRuntimeLabelHeightMode,
                         func::Maybe<FLevelRuntimeLabelSeed>>(
                  func::equals<ELevelRuntimeLabelHeightMode>(
                      ELevelRuntimeLabelHeightMode::LabelForScale),
                  [Request](ELevelRuntimeLabelHeightMode) {
                    return ReadReferenceScaleLabelSeed(Request);
                  }),
              func::when<ELevelRuntimeLabelHeightMode,
                         func::Maybe<FLevelRuntimeLabelSeed>>(
                  func::equals<ELevelRuntimeLabelHeightMode>(
                      ELevelRuntimeLabelHeightMode::AboveBlock),
                  [Request](ELevelRuntimeLabelHeightMode) {
                    return ReadReferenceScaleLabelSeed(Request);
                  }),
          });
  return func::match(
      Parsed,
      [](const func::Maybe<FLevelRuntimeLabelSeed> &Seed) { return Seed; },
      []() { return func::nothing<FLevelRuntimeLabelSeed>(); });
}

/**
 * @brief Validates required label fields and applies height-specific data.
 *
 * @signature func::Maybe<FLevelRuntimeLabelSeed> BuildLabelFromFields(const FLevelRuntimeLabelFieldsRequest &Request)
 *
 * User story: As a Level adapter maintainer, labels flow through one request
 * struct before becoming reducer input for unidirectional state flow.
 */
func::Maybe<FLevelRuntimeLabelSeed>
BuildLabelFromFields(const FLevelRuntimeLabelFieldsRequest &Request) {
  return Request.Id.hasValue && Request.Text.hasValue &&
                 Request.AnchorText.hasValue && Request.HeightText.hasValue &&
                 Request.EastLots.hasValue && Request.NorthLots.hasValue &&
                 Request.WorldSizeScale.hasValue
             ? ([Request]() {
                 const FLevelRuntimeEnumTextRequest AnchorRequest{
                     Request.AnchorText.value, TEXT("anchor")};
                 const func::Maybe<ELevelRuntimeAnchorMode> Anchor =
                     ParseAnchorMode(AnchorRequest);
                 return func::mbind(
                     Anchor, [Request](ELevelRuntimeAnchorMode ParsedAnchor) {
                       const FLevelRuntimeEnumTextRequest HeightRequest{
                           Request.HeightText.value, TEXT("height_mode")};
                       const func::Maybe<ELevelRuntimeLabelHeightMode> Height =
                           ParseLabelHeightMode(HeightRequest);
                       return func::mbind(
                           Height,
                           [Request, ParsedAnchor](
                               ELevelRuntimeLabelHeightMode ParsedHeight) {
                             return CompleteLabelHeight(
                                 {Request.Object,
                                  CreateLabelSeed({Request.Id.value,
                                                   Request.Text.value,
                                                   ParsedAnchor,
                                                   ParsedHeight,
                                                   Request.EastLots.value,
                                                   Request.NorthLots.value,
                                                   Request.WorldSizeScale
                                                       .value})});
                           });
                     });
               })()
             : func::nothing<FLevelRuntimeLabelSeed>();
}

} // namespace

func::Maybe<FLevelRuntimeLabelSeed>
LabelFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return BuildLabelFromFields(
      {Request.Object,
       JsonValues::ReadRequiredString({Request.Object, TEXT("id")}),
       JsonValues::ReadRequiredString({Request.Object, TEXT("text")}),
       JsonValues::ReadRequiredString({Request.Object, TEXT("anchor")}),
       JsonValues::ReadRequiredString({Request.Object, TEXT("height_mode")}),
       JsonValues::ReadRequiredFloat({Request.Object, TEXT("east_lots")}),
       JsonValues::ReadRequiredFloat({Request.Object, TEXT("north_lots")}),
       JsonValues::ReadRequiredFloat(
           {Request.Object, TEXT("world_size_scale")})});
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
