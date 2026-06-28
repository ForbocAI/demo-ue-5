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

struct FLevelRuntimeBlockSeedPayload {
  FString Id;
  FString Name;
  ELevelRuntimeAnchorMode Anchor = ELevelRuntimeAnchorMode::FeatureLots;
  FLevelRuntimeScaleSeed Scale;
  ELevelRetroTexture Texture = ELevelRetroTexture::MarkerPaint;
};

/**
 * @brief Creates a block seed before location-specific fields are applied.
 *
 * @signature FLevelRuntimeBlockSeed CreateBlockSeed(const FLevelRuntimeBlockSeedPayload &Payload)
 *
 * User story: As a Level adapter maintainer, validated block identity,
 * texture, anchor, and scale are grouped before location parsing.
 */
FLevelRuntimeBlockSeed
CreateBlockSeed(const FLevelRuntimeBlockSeedPayload &Payload) {
  FLevelRuntimeBlockSeed Seed;
  Seed.Id = Payload.Id;
  Seed.Name = Payload.Name;
  Seed.Anchor = Payload.Anchor;
  Seed.Scale = Payload.Scale;
  Seed.Texture = Payload.Texture;
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
      JsonValues::ReadRequiredObject(
          {Request.Object, TEXT("world_location")}),
      [Request](const TSharedPtr<FJsonObject> &WorldObject) {
        const FLevelRuntimeJsonObjectRequest LocationRequest{WorldObject};
        const func::Maybe<FVector> Location =
            WorldLocationFromJson(LocationRequest);
        return func::mbind(Location, [Request](const FVector &ParsedLocation) {
          FLevelRuntimeBlockSeed Seed = Request.Seed;
          Seed.WorldLocation = ParsedLocation;
          return func::just(Seed);
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
  const func::Maybe<float> EastLots =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("east_lots")});
  const func::Maybe<float> NorthLots =
      JsonValues::ReadRequiredFloat({Request.Object, TEXT("north_lots")});
  return EastLots.hasValue && NorthLots.hasValue
             ? ([Request, EastLots, NorthLots]() {
                 FLevelRuntimeBlockSeed Seed = Request.Seed;
                 Seed.EastLots = EastLots.value;
                 Seed.NorthLots = NorthLots.value;
                 return func::just(Seed);
               })()
             : func::nothing<FLevelRuntimeBlockSeed>();
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

/**
 * @brief Validates required block fields and applies anchor-specific location.
 *
 * @signature func::Maybe<FLevelRuntimeBlockSeed> BuildBlockFromFields(const FLevelRuntimeBlockFieldsRequest &Request)
 *
 * User story: As a Level adapter maintainer, all block fields flow through one
 * request struct before they become reducer input.
 */
func::Maybe<FLevelRuntimeBlockSeed>
BuildBlockFromFields(const FLevelRuntimeBlockFieldsRequest &Request) {
  return Request.Id.hasValue && Request.Name.hasValue &&
                 Request.AnchorText.hasValue && Request.Scale.hasValue &&
                 Request.TextureText.hasValue
             ? ([Request]() {
                 const FLevelRuntimeEnumTextRequest AnchorRequest{
                     Request.AnchorText.value, TEXT("anchor")};
                 const func::Maybe<ELevelRuntimeAnchorMode> Anchor =
                     ParseAnchorMode(AnchorRequest);
                 return func::mbind(
                     Anchor, [Request](ELevelRuntimeAnchorMode ParsedAnchor) {
                       const FLevelRuntimeEnumTextRequest TextureRequest{
                           Request.TextureText.value, TEXT("texture")};
                       const func::Maybe<ELevelRetroTexture> Texture =
                           ParseTexture(TextureRequest);
                       return func::mbind(
                           Texture,
                           [Request, ParsedAnchor](
                               ELevelRetroTexture ParsedTexture) {
                             return CompleteBlockLocation(
                                 {Request.Object,
                                  CreateBlockSeed({Request.Id.value,
                                                   Request.Name.value,
                                                   ParsedAnchor,
                                                   Request.Scale.value,
                                                   ParsedTexture})});
                           });
                     });
               })()
             : func::nothing<FLevelRuntimeBlockSeed>();
}

} // namespace

func::Maybe<FLevelRuntimeBlockSeed>
BlockFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return BuildBlockFromFields(
      {Request.Object,
       JsonValues::ReadRequiredString({Request.Object, TEXT("id")}),
       JsonValues::ReadRequiredString({Request.Object, TEXT("name")}),
       JsonValues::ReadRequiredString({Request.Object, TEXT("anchor")}),
       ReadScaleSeed({Request.Object, TEXT("scale")}),
       JsonValues::ReadRequiredString({Request.Object, TEXT("texture")})});
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
