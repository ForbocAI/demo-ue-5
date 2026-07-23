#include "Features/Systems/Level/Layout/Block/BlockAdapters.h"

#include "Features/Components/Data/Json/Value/JsonValueAdapters.h"
#include "Features/Systems/Level/Layout/Enum/EnumAdapters.h"
#include "Features/Systems/Level/Layout/Scale/ScaleAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

struct FLevelLotLocationSource {
  float EastLots;
  float NorthLots;
};

/** User Story: As a level layout block consumer, I need to invoke read block anchor mode field through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<EAnchorMode> ReadBlockAnchorModeField( const ForbocAI::Game::Data::FFieldRequest &Request) */
func::Maybe<EAnchorMode>
ReadBlockAnchorModeField(
    const ForbocAI::Game::Data::FFieldRequest &Request);

/** User Story: As a level layout block consumer, I need to invoke read texture field through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<ELevelRetroTexture> ReadTextureField(const ForbocAI::Game::Data::FFieldRequest &Request) */
func::Maybe<ELevelRetroTexture>
ReadTextureField(const ForbocAI::Game::Data::FFieldRequest &Request);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Game::Level::Layout::FLevelLotLocationSource,
    EastLots, NorthLots);

template <>
struct TRequiredJsonFieldRegistry<ForbocAI::Game::Level::FBlockSeed> {
  /** User Story: As a level layout block consumer, I need to invoke fields through a stable signature so the level layout block workflow remains explicit and composable. @fn static const TArray< TJsonFieldDeclaration< ForbocAI::Game::Level::FBlockSeed>> & Fields() */
  static const TArray<
      TJsonFieldDeclaration<
          ForbocAI::Game::Level::FBlockSeed>> &
  Fields() {
    static const TArray<
        TJsonFieldDeclaration<
            ForbocAI::Game::Level::FBlockSeed>>
        RegisteredFields = {
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FBlockSeed,
                                 Id, Name, YawDegrees),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FBlockSeed,
                ForbocAI::Game::Level::Layout::
                    ReadBlockAnchorModeField,
                Anchor),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FBlockSeed,
                ForbocAI::Game::Level::Layout::ReadScaleSeed, Scale),
            JSON_REQUIRED_FIELD_READER(
                ForbocAI::Game::Level::FBlockSeed,
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
using FFieldRequest = ForbocAI::Game::Data::FFieldRequest;

typedef func::Maybe<FBlockSeed> (*FBlockLocationReader)(
    const TSharedPtr<FJsonObject> &, const FBlockSeed &);

struct FLevelBlockLocationDeclaration {
  EAnchorMode Anchor;
  FBlockLocationReader Read;

  /** User Story: As a level layout block consumer, I need to invoke flevel block location declaration through a stable signature so the level layout block workflow remains explicit and composable. @fn FLevelBlockLocationDeclaration() = default */
  FLevelBlockLocationDeclaration() = default;

  /** User Story: As a level layout block consumer, I need to invoke flevel block location declaration through a stable signature so the level layout block workflow remains explicit and composable. @fn FLevelBlockLocationDeclaration(EAnchorMode InAnchor, FBlockLocationReader InRead) */
  FLevelBlockLocationDeclaration(EAnchorMode InAnchor,
                                        FBlockLocationReader InRead)
      : Anchor(InAnchor), Read(InRead) {}
};

/** User Story: As a level layout block consumer, I need to invoke assign world block location through a stable signature so the level layout block workflow remains explicit and composable. @fn FBlockSeed AssignWorldBlockLocation( const FBlockSeed &Seed, const FVector &Location) */
FBlockSeed AssignWorldBlockLocation(
    const FBlockSeed &Seed, const FVector &Location) {
  FBlockSeed Next = Seed;
  Next.WorldLocation = Location;
  return Next;
}

/** User Story: As a level layout block consumer, I need to invoke assign lot block location through a stable signature so the level layout block workflow remains explicit and composable. @fn FBlockSeed AssignLotBlockLocation( const FBlockSeed &Seed, const FLevelLotLocationSource &Fields) */
FBlockSeed AssignLotBlockLocation(
    const FBlockSeed &Seed,
    const FLevelLotLocationSource &Fields) {
  FBlockSeed Next = Seed;
  Next.EastLots = Fields.EastLots;
  Next.NorthLots = Fields.NorthLots;
  return Next;
}

/** User Story: As a level layout block consumer, I need to invoke read world block location through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<FBlockSeed> ReadWorldBlockLocation(const TSharedPtr<FJsonObject> &Object, const FBlockSeed &Seed) */
func::Maybe<FBlockSeed>
ReadWorldBlockLocation(const TSharedPtr<FJsonObject> &Object,
                       const FBlockSeed &Seed) {
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

/** User Story: As a level layout block consumer, I need to invoke read lot block location through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<FBlockSeed> ReadLotBlockLocation(const TSharedPtr<FJsonObject> &Object, const FBlockSeed &Seed) */
func::Maybe<FBlockSeed>
ReadLotBlockLocation(const TSharedPtr<FJsonObject> &Object,
                     const FBlockSeed &Seed) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FLevelLotLocationSource>({FLevelLotLocationSource(), Object}, JSON_REQUIRED_ATOMS(EastLots, NorthLots)),
      [Seed](const FLevelLotLocationSource &Fields) {
        return AssignLotBlockLocation(Seed, Fields);
      });
}

/** User Story: As a level layout block consumer, I need to invoke block location declarations through a stable signature so the level layout block workflow remains explicit and composable. @fn const TArray<FLevelBlockLocationDeclaration> & BlockLocationDeclarations() */
const TArray<FLevelBlockLocationDeclaration> &
BlockLocationDeclarations() {
  static const TArray<FLevelBlockLocationDeclaration> Declarations = {
      {EAnchorMode::World, ReadWorldBlockLocation},
      {EAnchorMode::BuildingLots, ReadLotBlockLocation},
      {EAnchorMode::FeatureLots, ReadLotBlockLocation},
      {EAnchorMode::PostOfficeLots, ReadLotBlockLocation}};
  return Declarations;
}

/** User Story: As a level layout block consumer, I need to invoke find block location declaration through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<FLevelBlockLocationDeclaration> FindBlockLocationDeclaration(EAnchorMode Anchor) */
func::Maybe<FLevelBlockLocationDeclaration>
FindBlockLocationDeclaration(EAnchorMode Anchor) {
  return func::find_array<FLevelBlockLocationDeclaration>(
      BlockLocationDeclarations(),
      [Anchor](const FLevelBlockLocationDeclaration &Declaration) {
        return Declaration.Anchor == Anchor;
      });
}

/** User Story: As a level layout block consumer, I need to invoke complete block location through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<FBlockSeed> CompleteBlockLocation(const TSharedPtr<FJsonObject> &Object, const FBlockSeed &Seed) */
func::Maybe<FBlockSeed>
CompleteBlockLocation(const TSharedPtr<FJsonObject> &Object,
                      const FBlockSeed &Seed) {
  return func::match(
      FindBlockLocationDeclaration(Seed.Anchor),
      [Object, Seed](const FLevelBlockLocationDeclaration
                         &Declaration) { return Declaration.Read(Object, Seed); },
      []() { return func::nothing<FBlockSeed>(); });
}

/** User Story: As a level layout block consumer, I need to invoke read block seed fields through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<FBlockSeed> ReadBlockSeedFields(const FLevelJsonObjectRequest &Request) */
func::Maybe<FBlockSeed>
ReadBlockSeedFields(const FLevelJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FBlockSeed>({FBlockSeed(), Request.Object}, JSON_REQUIRED_ATOMS(Id, Name, Anchor, YawDegrees, Scale, Texture));
}

} // namespace

/** User Story: As a level layout block consumer, I need to invoke read block anchor mode field through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<EAnchorMode> ReadBlockAnchorModeField(const FFieldRequest &Request) */
func::Maybe<EAnchorMode>
ReadBlockAnchorModeField(const FFieldRequest &Request) {
  return func::mbind(JsonValues::ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ParseAnchorMode({Text, Request.FieldName});
                     });
}

/** User Story: As a level layout block consumer, I need to invoke read texture field through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<ELevelRetroTexture> ReadTextureField(const FFieldRequest &Request) */
func::Maybe<ELevelRetroTexture>
ReadTextureField(const FFieldRequest &Request) {
  return func::mbind(JsonValues::ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ParseTexture({Text, Request.FieldName});
                     });
}

/** User Story: As a level layout block consumer, I need to invoke block from json through a stable signature so the level layout block workflow remains explicit and composable. @fn func::Maybe<FBlockSeed> BlockFromJson(const FLevelJsonObjectRequest &Request) */
func::Maybe<FBlockSeed>
BlockFromJson(const FLevelJsonObjectRequest &Request) {
  return func::mbind(
      ReadBlockSeedFields(Request),
      [Request](const FBlockSeed &Seed) {
        return CompleteBlockLocation(Request.Object, Seed);
      });
}

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
