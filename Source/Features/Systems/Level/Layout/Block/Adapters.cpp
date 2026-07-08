#include "Features/Systems/Level/Layout/Block/Adapters.h"

#include "Features/Components/Data/Json/Value/Adapters.h"
#include "Features/Systems/Level/Layout/Enum/Adapters.h"
#include "Features/Systems/Level/Layout/Scale/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

struct FLevelLotLocationFields {
  float EastLots;
  float NorthLots;
};

func::Maybe<ELevelAnchorMode>
ReadBlockAnchorModeField(
    const ForbocAI::Game::Data::FJsonFieldRequest &Request);

func::Maybe<ELevelRetroTexture>
ReadTextureField(const ForbocAI::Game::Data::FJsonFieldRequest &Request);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Game::Level::Layout::FLevelLotLocationFields,
    EastLots, NorthLots);

template <>
struct TRequiredJsonFieldRegistry<ForbocAI::Game::Level::FLevelBlockSeed> {
  static const TArray<
      TRequiredJsonFieldDeclaration<
          ForbocAI::Game::Level::FLevelBlockSeed>> &
  Fields() {
    static const TArray<
        TRequiredJsonFieldDeclaration<
            ForbocAI::Game::Level::FLevelBlockSeed>>
        RegisteredFields = {
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FLevelBlockSeed,
                                 Id, Name, YawDegrees),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FLevelBlockSeed,
                ForbocAI::Game::Level::Layout::
                    ReadBlockAnchorModeField,
                Anchor),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FLevelBlockSeed,
                ForbocAI::Game::Level::Layout::ReadScaleSeed, Scale),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FLevelBlockSeed,
                ForbocAI::Game::Level::Layout::ReadTextureField,
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
namespace Layout {
namespace {

namespace JsonValues = ForbocAI::Game::Data::JsonValueAdapters;
using FJsonFieldRequest = ForbocAI::Game::Data::FJsonFieldRequest;

typedef func::Maybe<FLevelBlockSeed> (*FBlockLocationReader)(
    const TSharedPtr<FJsonObject> &, const FLevelBlockSeed &);

struct FLevelBlockLocationDeclaration {
  ELevelAnchorMode Anchor;
  FBlockLocationReader Read;

  FLevelBlockLocationDeclaration() = default;

  FLevelBlockLocationDeclaration(ELevelAnchorMode InAnchor,
                                        FBlockLocationReader InRead)
      : Anchor(InAnchor), Read(InRead) {}
};

FLevelBlockSeed AssignWorldBlockLocation(
    const FLevelBlockSeed &Seed, const FVector &Location) {
  FLevelBlockSeed Next = Seed;
  Next.WorldLocation = Location;
  return Next;
}

FLevelBlockSeed AssignLotBlockLocation(
    const FLevelBlockSeed &Seed,
    const FLevelLotLocationFields &Fields) {
  FLevelBlockSeed Next = Seed;
  Next.EastLots = Fields.EastLots;
  Next.NorthLots = Fields.NorthLots;
  return Next;
}

func::Maybe<FLevelBlockSeed>
ReadWorldBlockLocation(const TSharedPtr<FJsonObject> &Object,
                       const FLevelBlockSeed &Seed) {
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

func::Maybe<FLevelBlockSeed>
ReadLotBlockLocation(const TSharedPtr<FJsonObject> &Object,
                     const FLevelBlockSeed &Seed) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FLevelLotLocationFields>({FLevelLotLocationFields(), Object}, JSON_REQUIRED_ATOMS(EastLots, NorthLots)),
      [Seed](const FLevelLotLocationFields &Fields) {
        return AssignLotBlockLocation(Seed, Fields);
      });
}

const TArray<FLevelBlockLocationDeclaration> &
BlockLocationDeclarations() {
  static const TArray<FLevelBlockLocationDeclaration> Declarations = {
      {ELevelAnchorMode::World, ReadWorldBlockLocation},
      {ELevelAnchorMode::BuildingLots, ReadLotBlockLocation},
      {ELevelAnchorMode::FeatureLots, ReadLotBlockLocation},
      {ELevelAnchorMode::PostOfficeLots, ReadLotBlockLocation}};
  return Declarations;
}

func::Maybe<FLevelBlockLocationDeclaration>
FindBlockLocationDeclaration(ELevelAnchorMode Anchor) {
  return func::find_array<FLevelBlockLocationDeclaration>(
      BlockLocationDeclarations(),
      [Anchor](const FLevelBlockLocationDeclaration &Declaration) {
        return Declaration.Anchor == Anchor;
      });
}

func::Maybe<FLevelBlockSeed>
CompleteBlockLocation(const TSharedPtr<FJsonObject> &Object,
                      const FLevelBlockSeed &Seed) {
  return func::match(
      FindBlockLocationDeclaration(Seed.Anchor),
      [Object, Seed](const FLevelBlockLocationDeclaration
                         &Declaration) { return Declaration.Read(Object, Seed); },
      []() { return func::nothing<FLevelBlockSeed>(); });
}

func::Maybe<FLevelBlockSeed>
ReadBlockSeedFields(const FLevelJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelBlockSeed>({FLevelBlockSeed(), Request.Object}, JSON_REQUIRED_ATOMS(Id, Name, Anchor, YawDegrees, Scale, Texture));
}

} // namespace

func::Maybe<ELevelAnchorMode>
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

func::Maybe<FLevelBlockSeed>
BlockFromJson(const FLevelJsonObjectRequest &Request) {
  return func::mbind(
      ReadBlockSeedFields(Request),
      [Request](const FLevelBlockSeed &Seed) {
        return CompleteBlockLocation(Request.Object, Seed);
      });
}

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
