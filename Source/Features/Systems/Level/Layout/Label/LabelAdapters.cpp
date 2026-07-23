#include "Features/Systems/Level/Layout/Label/LabelAdapters.h"

#include "Features/Components/Data/Json/Value/JsonValueAdapters.h"
#include "Features/Systems/Level/Layout/Enum/EnumAdapters.h"
#include "Features/Systems/Level/Layout/Scale/ScaleAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

struct FLevelLabelExplicitHeightSource {
  float HeightOffset;
};

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

template <>
func::Maybe<ForbocAI::Game::Level::EAnchorMode>
ReadRequiredValue<ForbocAI::Game::Level::EAnchorMode>(
    const ForbocAI::Game::Data::FFieldRequest &Request) {
  return func::mbind(ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ForbocAI::Game::Level::Layout::
                           ParseAnchorMode({Text, Request.FieldName});
                     });
}

template <>
func::Maybe<ForbocAI::Game::Level::ELabelHeightMode>
ReadRequiredValue<ForbocAI::Game::Level::ELabelHeightMode>(
    const ForbocAI::Game::Data::FFieldRequest &Request) {
  return func::mbind(ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ForbocAI::Game::Level::Layout::
                           ParseLabelHeightMode({Text, Request.FieldName});
                     });
}

template <>
struct TRequiredJsonFieldRegistry<
    ForbocAI::Game::Level::FLabelSeed> {
  /** User Story: As a level layout label consumer, I need to invoke fields through a stable signature so the level layout label workflow remains explicit and composable. @fn static const TArray< TJsonFieldDeclaration< ForbocAI::Game::Level::FLabelSeed>> & Fields() */
  static const TArray<
      TJsonFieldDeclaration<
          ForbocAI::Game::Level::FLabelSeed>> &
  Fields() {
    static const TArray<
        TJsonFieldDeclaration<
            ForbocAI::Game::Level::FLabelSeed>>
        RegisteredFields = {
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FLabelSeed,
                                 Id, Text, Anchor),
            {"HeightMode",
             &ForbocAI::Game::Level::FLabelSeed::Height},
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FLabelSeed,
                                 EastLots, NorthLots, WorldSizeScale)};
    return RegisteredFields;
  }
};

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Game::Level::Layout::
        FLevelLabelExplicitHeightSource,
    HeightOffset);

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

typedef func::Maybe<FLabelSeed> (*FLabelHeightReader)(
    const TSharedPtr<FJsonObject> &, const FLabelSeed &);

struct FLevelLabelHeightDeclaration {
  ELabelHeightMode Height;
  FLabelHeightReader Read;

  /** User Story: As a level layout label consumer, I need to invoke flevel label height declaration through a stable signature so the level layout label workflow remains explicit and composable. @fn FLevelLabelHeightDeclaration() = default */
  FLevelLabelHeightDeclaration() = default;

  /** User Story: As a level layout label consumer, I need to invoke flevel label height declaration through a stable signature so the level layout label workflow remains explicit and composable. @fn FLevelLabelHeightDeclaration(ELabelHeightMode InHeight, FLabelHeightReader InRead) */
  FLevelLabelHeightDeclaration(ELabelHeightMode InHeight,
                                      FLabelHeightReader InRead)
      : Height(InHeight), Read(InRead) {}
};

/** User Story: As a level layout label consumer, I need to invoke assign explicit height through a stable signature so the level layout label workflow remains explicit and composable. @fn FLabelSeed AssignExplicitHeight(const FLabelSeed &Seed, const FLevelLabelExplicitHeightSource &Fields) */
FLabelSeed
AssignExplicitHeight(const FLabelSeed &Seed,
                     const FLevelLabelExplicitHeightSource &Fields) {
  FLabelSeed Next = Seed;
  Next.HeightOffset = Fields.HeightOffset;
  return Next;
}

/** User Story: As a level layout label consumer, I need to invoke assign reference scale through a stable signature so the level layout label workflow remains explicit and composable. @fn FLabelSeed AssignReferenceScale(const FLabelSeed &Seed, const FScaleSeed &ReferenceScale) */
FLabelSeed
AssignReferenceScale(const FLabelSeed &Seed,
                     const FScaleSeed &ReferenceScale) {
  FLabelSeed Next = Seed;
  Next.ReferenceScale = ReferenceScale;
  return Next;
}

/** User Story: As a level layout label consumer, I need to invoke read explicit height label seed through a stable signature so the level layout label workflow remains explicit and composable. @fn func::Maybe<FLabelSeed> ReadExplicitHeightLabelSeed(const TSharedPtr<FJsonObject> &Object, const FLabelSeed &Seed) */
func::Maybe<FLabelSeed>
ReadExplicitHeightLabelSeed(const TSharedPtr<FJsonObject> &Object,
                            const FLabelSeed &Seed) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FLevelLabelExplicitHeightSource>({FLevelLabelExplicitHeightSource(), Object}, JSON_REQUIRED_ATOMS(HeightOffset)),
      [Seed](const FLevelLabelExplicitHeightSource &Fields) {
        return AssignExplicitHeight(Seed, Fields);
      });
}

/** User Story: As a level layout label consumer, I need to invoke read reference scale label seed through a stable signature so the level layout label workflow remains explicit and composable. @fn func::Maybe<FLabelSeed> ReadReferenceScaleLabelSeed(const TSharedPtr<FJsonObject> &Object, const FLabelSeed &Seed) */
func::Maybe<FLabelSeed>
ReadReferenceScaleLabelSeed(const TSharedPtr<FJsonObject> &Object,
                            const FLabelSeed &Seed) {
  return func::fmap(
      ReadScaleSeed(JsonValues::RequiredField(Object, "ReferenceScale")),
      [Seed](const FScaleSeed &ReferenceScale) {
        return AssignReferenceScale(Seed, ReferenceScale);
      });
}

/** User Story: As a level layout label consumer, I need to invoke label height declarations through a stable signature so the level layout label workflow remains explicit and composable. @fn const TArray<FLevelLabelHeightDeclaration> &LabelHeightDeclarations() */
const TArray<FLevelLabelHeightDeclaration> &LabelHeightDeclarations() {
  static const TArray<FLevelLabelHeightDeclaration> Declarations = {
      {ELabelHeightMode::Explicit, ReadExplicitHeightLabelSeed},
      {ELabelHeightMode::LabelForScale,
       ReadReferenceScaleLabelSeed},
      {ELabelHeightMode::AboveBlock, ReadReferenceScaleLabelSeed}};
  return Declarations;
}

/** User Story: As a level layout label consumer, I need to invoke find label height declaration through a stable signature so the level layout label workflow remains explicit and composable. @fn func::Maybe<FLevelLabelHeightDeclaration> FindLabelHeightDeclaration(ELabelHeightMode Height) */
func::Maybe<FLevelLabelHeightDeclaration>
FindLabelHeightDeclaration(ELabelHeightMode Height) {
  return func::find_array<FLevelLabelHeightDeclaration>(
      LabelHeightDeclarations(),
      [Height](const FLevelLabelHeightDeclaration &Declaration) {
        return Declaration.Height == Height;
      });
}

/** User Story: As a level layout label consumer, I need to invoke complete label height through a stable signature so the level layout label workflow remains explicit and composable. @fn func::Maybe<FLabelSeed> CompleteLabelHeight(const TSharedPtr<FJsonObject> &Object, const FLabelSeed &Seed) */
func::Maybe<FLabelSeed>
CompleteLabelHeight(const TSharedPtr<FJsonObject> &Object,
                    const FLabelSeed &Seed) {
  return func::match(
      FindLabelHeightDeclaration(Seed.Height),
      [Object, Seed](const FLevelLabelHeightDeclaration &Declaration) {
        return Declaration.Read(Object, Seed);
      },
      []() { return func::nothing<FLabelSeed>(); });
}

/** User Story: As a level layout label consumer, I need to invoke read label seed fields through a stable signature so the level layout label workflow remains explicit and composable. @fn func::Maybe<FLabelSeed> ReadLabelSeedFields(const FLevelJsonObjectRequest &Request) */
func::Maybe<FLabelSeed>
ReadLabelSeedFields(const FLevelJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLabelSeed>({FLabelSeed(), Request.Object}, JSON_REQUIRED_ATOMS(Id, Text, Anchor, HeightMode,
                                          EastLots, NorthLots, WorldSizeScale));
}

} // namespace

/** User Story: As a level layout label consumer, I need to invoke label from json through a stable signature so the level layout label workflow remains explicit and composable. @fn func::Maybe<FLabelSeed> LabelFromJson(const FLevelJsonObjectRequest &Request) */
func::Maybe<FLabelSeed>
LabelFromJson(const FLevelJsonObjectRequest &Request) {
  return func::mbind(
      ReadLabelSeedFields(Request),
      [Request](const FLabelSeed &Seed) {
        return CompleteLabelHeight(Request.Object, Seed);
      });
}

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
