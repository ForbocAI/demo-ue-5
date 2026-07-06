#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeBlockAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeScaleAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {

struct FLevelRuntimeLotLocationFields {
  float EastLots;
  float NorthLots;
};

func::Maybe<ELevelRuntimeAnchorMode>
ReadBlockAnchorModeField(
    const ForbocAI::Game::Data::FJsonFieldRequest &Request);

func::Maybe<ELevelRetroTexture>
ReadTextureField(const ForbocAI::Game::Data::FJsonFieldRequest &Request);

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Game::Level::RuntimeLayout::FLevelRuntimeLotLocationFields,
    EastLots, NorthLots);

template <>
struct TRequiredJsonFieldRegistry<ForbocAI::Game::Level::FLevelRuntimeBlockSeed> {
  static const TArray<
      TRequiredJsonFieldDeclaration<
          ForbocAI::Game::Level::FLevelRuntimeBlockSeed>> &
  Fields() {
    static const TArray<
        TRequiredJsonFieldDeclaration<
            ForbocAI::Game::Level::FLevelRuntimeBlockSeed>>
        RegisteredFields = {
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FLevelRuntimeBlockSeed,
                                 Id, Name, YawDegrees),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FLevelRuntimeBlockSeed,
                ForbocAI::Game::Level::RuntimeLayout::
                    ReadBlockAnchorModeField,
                Anchor),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FLevelRuntimeBlockSeed,
                ForbocAI::Game::Level::RuntimeLayout::ReadScaleSeed, Scale),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FLevelRuntimeBlockSeed,
                ForbocAI::Game::Level::RuntimeLayout::ReadTextureField,
                Texture)};
    return RegisteredFields;
  }
};

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonValues = ForbocAI::Game::Data::JsonValueAdapters;
using FJsonFieldRequest = ForbocAI::Game::Data::FJsonFieldRequest;

typedef func::Maybe<FLevelRuntimeBlockSeed> (*FBlockLocationReader)(
    const TSharedPtr<FJsonObject> &, const FLevelRuntimeBlockSeed &);

struct FLevelRuntimeBlockLocationDeclaration {
  ELevelRuntimeAnchorMode Anchor;
  FBlockLocationReader Read;

  FLevelRuntimeBlockLocationDeclaration() = default;

  FLevelRuntimeBlockLocationDeclaration(ELevelRuntimeAnchorMode InAnchor,
                                        FBlockLocationReader InRead)
      : Anchor(InAnchor), Read(InRead) {}
};

FLevelRuntimeBlockSeed AssignWorldBlockLocation(
    const FLevelRuntimeBlockSeed &Seed, const FVector &Location) {
  FLevelRuntimeBlockSeed Next = Seed;
  Next.WorldLocation = Location;
  return Next;
}

FLevelRuntimeBlockSeed AssignLotBlockLocation(
    const FLevelRuntimeBlockSeed &Seed,
    const FLevelRuntimeLotLocationFields &Fields) {
  FLevelRuntimeBlockSeed Next = Seed;
  Next.EastLots = Fields.EastLots;
  Next.NorthLots = Fields.NorthLots;
  return Next;
}

func::Maybe<FLevelRuntimeBlockSeed>
ReadWorldBlockLocation(const TSharedPtr<FJsonObject> &Object,
                       const FLevelRuntimeBlockSeed &Seed) {
  return func::mbind(
      JsonValues::ReadRequiredField<TSharedPtr<FJsonObject>>(Object,
                                                             "WorldLocation"),
      [Seed](const TSharedPtr<FJsonObject> &WorldObject) {
        return func::fmap(
            WorldLocationFromJson({WorldObject}),
            [Seed](const FVector &Location) {
              return AssignWorldBlockLocation(Seed, Location);
            });
      });
}

func::Maybe<FLevelRuntimeBlockSeed>
ReadLotBlockLocation(const TSharedPtr<FJsonObject> &Object,
                     const FLevelRuntimeBlockSeed &Seed) {
  return func::fmap(
      JsonValues::ReduceRequiredFields<FLevelRuntimeLotLocationFields>(FLevelRuntimeLotLocationFields(), Object, JSON_REQUIRED_ATOMS(EastLots, NorthLots)),
      [Seed](const FLevelRuntimeLotLocationFields &Fields) {
        return AssignLotBlockLocation(Seed, Fields);
      });
}

const TArray<FLevelRuntimeBlockLocationDeclaration> &
BlockLocationDeclarations() {
  static const TArray<FLevelRuntimeBlockLocationDeclaration> Declarations = {
      {ELevelRuntimeAnchorMode::World, ReadWorldBlockLocation},
      {ELevelRuntimeAnchorMode::BuildingLots, ReadLotBlockLocation},
      {ELevelRuntimeAnchorMode::FeatureLots, ReadLotBlockLocation},
      {ELevelRuntimeAnchorMode::PostOfficeLots, ReadLotBlockLocation}};
  return Declarations;
}

func::Maybe<FLevelRuntimeBlockLocationDeclaration>
FindBlockLocationDeclaration(ELevelRuntimeAnchorMode Anchor) {
  return func::find_array<FLevelRuntimeBlockLocationDeclaration>(
      BlockLocationDeclarations(),
      [Anchor](const FLevelRuntimeBlockLocationDeclaration &Declaration) {
        return Declaration.Anchor == Anchor;
      });
}

func::Maybe<FLevelRuntimeBlockSeed>
CompleteBlockLocation(const TSharedPtr<FJsonObject> &Object,
                      const FLevelRuntimeBlockSeed &Seed) {
  return func::match(
      FindBlockLocationDeclaration(Seed.Anchor),
      [Object, Seed](const FLevelRuntimeBlockLocationDeclaration
                         &Declaration) { return Declaration.Read(Object, Seed); },
      []() { return func::nothing<FLevelRuntimeBlockSeed>(); });
}

func::Maybe<FLevelRuntimeBlockSeed>
ReadBlockSeedFields(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReduceRequiredFields<FLevelRuntimeBlockSeed>(FLevelRuntimeBlockSeed(), Request.Object, JSON_REQUIRED_ATOMS(Id, Name, Anchor, YawDegrees, Scale, Texture));
}

} // namespace

func::Maybe<ELevelRuntimeAnchorMode>
ReadBlockAnchorModeField(const FJsonFieldRequest &Request) {
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

func::Maybe<FLevelRuntimeBlockSeed>
BlockFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::mbind(
      ReadBlockSeedFields(Request),
      [Request](const FLevelRuntimeBlockSeed &Seed) {
        return CompleteBlockLocation(Request.Object, Seed);
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
