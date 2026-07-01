#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeBlockAdapters.h"

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

struct FLevelRuntimeLotLocationFields {
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
};

struct FLevelRuntimeBlockLocationRequest {
  TSharedPtr<FJsonObject> Object;
  FLevelRuntimeBlockSeed Seed;
};

func::Maybe<ELevelRuntimeAnchorMode>
ReadAnchorModeField(const FJsonFieldRequest &Request) {
  return func::mbind(JsonValues::ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ParseAnchorMode({Text, Request.FieldName});
                     });
}

func::Maybe<ELevelRetroTexture>
ReadTextureField(const FJsonFieldRequest &Request) {
  return func::mbind(JsonValues::ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ParseTexture({Text, Request.FieldName});
                     });
}

FLevelRuntimeBlockSeed
ApplyWorldBlockLocation(const FLevelRuntimeBlockLocationRequest &Request,
                        const FVector &Location) {
  FLevelRuntimeBlockSeed Seed = Request.Seed;
  Seed.WorldLocation = Location;
  return Seed;
}

FLevelRuntimeBlockSeed
ApplyLotBlockLocation(const FLevelRuntimeBlockLocationRequest &Request,
                      const FLevelRuntimeLotLocationFields &Fields) {
  FLevelRuntimeBlockSeed Seed = Request.Seed;
  Seed.EastLots = Fields.EastLots;
  Seed.NorthLots = Fields.NorthLots;
  return Seed;
}

/**
 * @brief Applies world-location data to a block seed.
 *
 * @signature func::Maybe<FLevelRuntimeBlockSeed> ReadWorldBlockSeed(const FLevelRuntimeBlockLocationRequest &Request)
 *
 * User story: As a data author, world-anchored blocks can declare explicit
 * coordinates while reducers remain free of JSON decisions.
 */
func::Maybe<FLevelRuntimeBlockSeed>
ReadWorldBlockSeed(const FLevelRuntimeBlockLocationRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredField<TSharedPtr<FJsonObject>>(
          Request.Object, "WorldLocation"),
      [Request](const TSharedPtr<FJsonObject> &WorldObject) {
        return func::fmap(
            WorldLocationFromJson({WorldObject}),
            [Request](const FVector &Location) {
              return ApplyWorldBlockLocation(Request, Location);
            });
      });
}

/**
 * @brief Applies post-office lot coordinates to a block seed.
 *
 * @signature func::Maybe<FLevelRuntimeBlockSeed> ReadLotBlockSeed(const FLevelRuntimeBlockLocationRequest &Request)
 *
 * User story: As a data author, lot-anchored blocks are validated into typed
 * seed coordinates before reducer-owned terrain projection.
 */
func::Maybe<FLevelRuntimeBlockSeed>
ReadLotBlockSeed(const FLevelRuntimeBlockLocationRequest &Request) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FLevelRuntimeLotLocationFields>(
          Request.Object,
          {JSON_REQUIRED_FIELDS(FLevelRuntimeLotLocationFields, EastLots,
                                NorthLots)}),
      [Request](const FLevelRuntimeLotLocationFields &Fields) {
        return ApplyLotBlockLocation(Request, Fields);
      });
}

/**
 * @brief Completes a block seed according to its validated anchor mode.
 *
 * @signature func::Maybe<FLevelRuntimeBlockSeed> CompleteBlockLocation(const FLevelRuntimeBlockLocationRequest &Request)
 *
 * User story: As an ECS system maintainer, anchor branching remains pure
 * adapter logic and does not enter views or store ownership.
 */
func::Maybe<FLevelRuntimeBlockSeed>
CompleteBlockLocation(const FLevelRuntimeBlockLocationRequest &Request) {
  const func::Maybe<func::Maybe<FLevelRuntimeBlockSeed>> Parsed =
      func::multi_match<ELevelRuntimeAnchorMode,
                        func::Maybe<FLevelRuntimeBlockSeed>>(
          Request.Seed.Anchor,
          {
              func::when<ELevelRuntimeAnchorMode,
                         func::Maybe<FLevelRuntimeBlockSeed>>(
                  func::equals<ELevelRuntimeAnchorMode>(
                      ELevelRuntimeAnchorMode::World),
                  [Request](ELevelRuntimeAnchorMode) {
                    return ReadWorldBlockSeed(Request);
                  }),
              func::when<ELevelRuntimeAnchorMode,
                         func::Maybe<FLevelRuntimeBlockSeed>>(
                  func::equals<ELevelRuntimeAnchorMode>(
                      ELevelRuntimeAnchorMode::BuildingLots),
                  [Request](ELevelRuntimeAnchorMode) {
                    return ReadLotBlockSeed(Request);
                  }),
              func::when<ELevelRuntimeAnchorMode,
                         func::Maybe<FLevelRuntimeBlockSeed>>(
                  func::equals<ELevelRuntimeAnchorMode>(
                      ELevelRuntimeAnchorMode::FeatureLots),
                  [Request](ELevelRuntimeAnchorMode) {
                    return ReadLotBlockSeed(Request);
                  }),
              func::when<ELevelRuntimeAnchorMode,
                         func::Maybe<FLevelRuntimeBlockSeed>>(
                  func::equals<ELevelRuntimeAnchorMode>(
                      ELevelRuntimeAnchorMode::PostOfficeLots),
                  [Request](ELevelRuntimeAnchorMode) {
                    return ReadLotBlockSeed(Request);
                  }),
          });
  return func::match(
      Parsed,
      [](const func::Maybe<FLevelRuntimeBlockSeed> &Seed) { return Seed; },
      []() { return func::nothing<FLevelRuntimeBlockSeed>(); });
}

func::Maybe<FLevelRuntimeBlockSeed>
ReadBlockSeedFields(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeBlockSeed>(
      Request.Object,
      {JSON_REQUIRED_FIELDS(FLevelRuntimeBlockSeed, Id, Name),
       JSON_REQUIRED_FIELD_READER(FLevelRuntimeBlockSeed, ReadAnchorModeField,
                                  Anchor),
       JSON_REQUIRED_FIELD_READER(FLevelRuntimeBlockSeed, ReadScaleSeed, Scale),
       JSON_REQUIRED_FIELD_READER(FLevelRuntimeBlockSeed, ReadTextureField,
                                  Texture)});
}

} // namespace

func::Maybe<FLevelRuntimeBlockSeed>
BlockFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::mbind(
      ReadBlockSeedFields(Request),
      [Request](const FLevelRuntimeBlockSeed &Seed) {
        return CompleteBlockLocation({Request.Object, Seed});
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
