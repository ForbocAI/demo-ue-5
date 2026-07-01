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
using FJsonFieldRequest = ForbocAI::Demo::Data::FJsonFieldRequest;

struct FLevelRuntimeLabelHeightRequest {
  TSharedPtr<FJsonObject> Object;
  FLevelRuntimeLabelSeed Seed;
};

func::Maybe<ELevelRuntimeAnchorMode>
ReadLabelAnchorModeField(const FJsonFieldRequest &Request) {
  return func::mbind(JsonValues::ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ParseAnchorMode({Text, Request.FieldName});
                     });
}

func::Maybe<ELevelRuntimeLabelHeightMode>
ReadLabelHeightModeField(const FJsonFieldRequest &Request) {
  return func::mbind(JsonValues::ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ParseLabelHeightMode({Text, Request.FieldName});
                     });
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
      JsonValues::ReadRequiredField<float>(Request.Object, "HeightOffset"),
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
      ReadScaleSeed(JsonValues::RequiredField(Request.Object,
                                              "ReferenceScale")),
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

func::Maybe<FLevelRuntimeLabelSeed>
ReadLabelSeedFields(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeLabelSeed>(
      Request.Object,
      {JSON_REQUIRED_FIELDS(FLevelRuntimeLabelSeed, Id, Text, EastLots,
                            NorthLots, WorldSizeScale),
       JSON_REQUIRED_FIELD_READER(FLevelRuntimeLabelSeed,
                                  ReadLabelAnchorModeField, Anchor),
       JSON_REQUIRED_FIELD_READER(FLevelRuntimeLabelSeed,
                                  ReadLabelHeightModeField, Height)});
}

} // namespace

func::Maybe<FLevelRuntimeLabelSeed>
LabelFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::mbind(
      ReadLabelSeedFields(Request),
      [Request](const FLevelRuntimeLabelSeed &Seed) {
        return CompleteLabelHeight({Request.Object, Seed});
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
