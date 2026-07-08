#include "Features/Systems/Level/Layout/Label/Adapters.h"

#include "Features/Components/Data/Json/Value/Adapters.h"
#include "Features/Systems/Level/Layout/Enum/Adapters.h"
#include "Features/Systems/Level/Layout/Scale/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

struct FLevelLabelExplicitHeightFields {
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
func::Maybe<ForbocAI::Game::Level::ELevelAnchorMode>
ReadRequiredValue<ForbocAI::Game::Level::ELevelAnchorMode>(
    const ForbocAI::Game::Data::FJsonFieldRequest &Request) {
  return func::mbind(ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ForbocAI::Game::Level::Layout::
                           ParseAnchorMode({Text, Request.FieldName});
                     });
}

template <>
func::Maybe<ForbocAI::Game::Level::ELevelLabelHeightMode>
ReadRequiredValue<ForbocAI::Game::Level::ELevelLabelHeightMode>(
    const ForbocAI::Game::Data::FJsonFieldRequest &Request) {
  return func::mbind(ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ForbocAI::Game::Level::Layout::
                           ParseLabelHeightMode({Text, Request.FieldName});
                     });
}

template <>
struct TRequiredJsonFieldRegistry<
    ForbocAI::Game::Level::FLevelLabelSeed> {
  static const TArray<
      TRequiredJsonFieldDeclaration<
          ForbocAI::Game::Level::FLevelLabelSeed>> &
  Fields() {
    static const TArray<
        TRequiredJsonFieldDeclaration<
            ForbocAI::Game::Level::FLevelLabelSeed>>
        RegisteredFields = {
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FLevelLabelSeed,
                                 Id, Text, Anchor),
            {"HeightMode",
             &ForbocAI::Game::Level::FLevelLabelSeed::Height},
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FLevelLabelSeed,
                                 EastLots, NorthLots, WorldSizeScale)};
    return RegisteredFields;
  }
};

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Game::Level::Layout::
        FLevelLabelExplicitHeightFields,
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

typedef func::Maybe<FLevelLabelSeed> (*FLabelHeightReader)(
    const TSharedPtr<FJsonObject> &, const FLevelLabelSeed &);

struct FLevelLabelHeightDeclaration {
  ELevelLabelHeightMode Height;
  FLabelHeightReader Read;

  FLevelLabelHeightDeclaration() = default;

  FLevelLabelHeightDeclaration(ELevelLabelHeightMode InHeight,
                                      FLabelHeightReader InRead)
      : Height(InHeight), Read(InRead) {}
};

FLevelLabelSeed
AssignExplicitHeight(const FLevelLabelSeed &Seed,
                     const FLevelLabelExplicitHeightFields &Fields) {
  FLevelLabelSeed Next = Seed;
  Next.HeightOffset = Fields.HeightOffset;
  return Next;
}

FLevelLabelSeed
AssignReferenceScale(const FLevelLabelSeed &Seed,
                     const FLevelScaleSeed &ReferenceScale) {
  FLevelLabelSeed Next = Seed;
  Next.ReferenceScale = ReferenceScale;
  return Next;
}

func::Maybe<FLevelLabelSeed>
ReadExplicitHeightLabelSeed(const TSharedPtr<FJsonObject> &Object,
                            const FLevelLabelSeed &Seed) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FLevelLabelExplicitHeightFields>({FLevelLabelExplicitHeightFields(), Object}, JSON_REQUIRED_ATOMS(HeightOffset)),
      [Seed](const FLevelLabelExplicitHeightFields &Fields) {
        return AssignExplicitHeight(Seed, Fields);
      });
}

func::Maybe<FLevelLabelSeed>
ReadReferenceScaleLabelSeed(const TSharedPtr<FJsonObject> &Object,
                            const FLevelLabelSeed &Seed) {
  return func::fmap(
      ReadScaleSeed(JsonValues::RequiredField(Object, "ReferenceScale")),
      [Seed](const FLevelScaleSeed &ReferenceScale) {
        return AssignReferenceScale(Seed, ReferenceScale);
      });
}

const TArray<FLevelLabelHeightDeclaration> &LabelHeightDeclarations() {
  static const TArray<FLevelLabelHeightDeclaration> Declarations = {
      {ELevelLabelHeightMode::Explicit, ReadExplicitHeightLabelSeed},
      {ELevelLabelHeightMode::LabelForScale,
       ReadReferenceScaleLabelSeed},
      {ELevelLabelHeightMode::AboveBlock, ReadReferenceScaleLabelSeed}};
  return Declarations;
}

func::Maybe<FLevelLabelHeightDeclaration>
FindLabelHeightDeclaration(ELevelLabelHeightMode Height) {
  return func::find_array<FLevelLabelHeightDeclaration>(
      LabelHeightDeclarations(),
      [Height](const FLevelLabelHeightDeclaration &Declaration) {
        return Declaration.Height == Height;
      });
}

func::Maybe<FLevelLabelSeed>
CompleteLabelHeight(const TSharedPtr<FJsonObject> &Object,
                    const FLevelLabelSeed &Seed) {
  return func::match(
      FindLabelHeightDeclaration(Seed.Height),
      [Object, Seed](const FLevelLabelHeightDeclaration &Declaration) {
        return Declaration.Read(Object, Seed);
      },
      []() { return func::nothing<FLevelLabelSeed>(); });
}

func::Maybe<FLevelLabelSeed>
ReadLabelSeedFields(const FLevelJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelLabelSeed>({FLevelLabelSeed(), Request.Object}, JSON_REQUIRED_ATOMS(Id, Text, Anchor, HeightMode,
                                          EastLots, NorthLots, WorldSizeScale));
}

} // namespace

func::Maybe<FLevelLabelSeed>
LabelFromJson(const FLevelJsonObjectRequest &Request) {
  return func::mbind(
      ReadLabelSeedFields(Request),
      [Request](const FLevelLabelSeed &Seed) {
        return CompleteLabelHeight(Request.Object, Seed);
      });
}

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
