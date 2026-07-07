#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLabelAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeScaleAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {

struct FLevelRuntimeLabelExplicitHeightFields {
  float HeightOffset;
};

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

template <>
func::Maybe<ForbocAI::Game::Level::ELevelRuntimeAnchorMode>
ReadRequiredValue<ForbocAI::Game::Level::ELevelRuntimeAnchorMode>(
    const ForbocAI::Game::Data::FJsonFieldRequest &Request) {
  return func::mbind(ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ForbocAI::Game::Level::RuntimeLayout::
                           ParseAnchorMode({Text, Request.FieldName});
                     });
}

template <>
func::Maybe<ForbocAI::Game::Level::ELevelRuntimeLabelHeightMode>
ReadRequiredValue<ForbocAI::Game::Level::ELevelRuntimeLabelHeightMode>(
    const ForbocAI::Game::Data::FJsonFieldRequest &Request) {
  return func::mbind(ReadRequiredValue<FString>(Request),
                     [Request](const FString &Text) {
                       return ForbocAI::Game::Level::RuntimeLayout::
                           ParseLabelHeightMode({Text, Request.FieldName});
                     });
}

template <>
struct TRequiredJsonFieldRegistry<
    ForbocAI::Game::Level::FLevelRuntimeLabelSeed> {
  static const TArray<
      TRequiredJsonFieldDeclaration<
          ForbocAI::Game::Level::FLevelRuntimeLabelSeed>> &
  Fields() {
    static const TArray<
        TRequiredJsonFieldDeclaration<
            ForbocAI::Game::Level::FLevelRuntimeLabelSeed>>
        RegisteredFields = {
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FLevelRuntimeLabelSeed,
                                 Id, Text, Anchor),
            {"HeightMode",
             &ForbocAI::Game::Level::FLevelRuntimeLabelSeed::Height},
            JSON_REQUIRED_FIELDS(ForbocAI::Game::Level::FLevelRuntimeLabelSeed,
                                 EastLots, NorthLots, WorldSizeScale)};
    return RegisteredFields;
  }
};

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Game::Level::RuntimeLayout::
        FLevelRuntimeLabelExplicitHeightFields,
    HeightOffset);

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

typedef func::Maybe<FLevelRuntimeLabelSeed> (*FLabelHeightReader)(
    const TSharedPtr<FJsonObject> &, const FLevelRuntimeLabelSeed &);

struct FLevelRuntimeLabelHeightDeclaration {
  ELevelRuntimeLabelHeightMode Height;
  FLabelHeightReader Read;

  FLevelRuntimeLabelHeightDeclaration() = default;

  FLevelRuntimeLabelHeightDeclaration(ELevelRuntimeLabelHeightMode InHeight,
                                      FLabelHeightReader InRead)
      : Height(InHeight), Read(InRead) {}
};

FLevelRuntimeLabelSeed
AssignExplicitHeight(const FLevelRuntimeLabelSeed &Seed,
                     const FLevelRuntimeLabelExplicitHeightFields &Fields) {
  FLevelRuntimeLabelSeed Next = Seed;
  Next.HeightOffset = Fields.HeightOffset;
  return Next;
}

FLevelRuntimeLabelSeed
AssignReferenceScale(const FLevelRuntimeLabelSeed &Seed,
                     const FLevelRuntimeScaleSeed &ReferenceScale) {
  FLevelRuntimeLabelSeed Next = Seed;
  Next.ReferenceScale = ReferenceScale;
  return Next;
}

func::Maybe<FLevelRuntimeLabelSeed>
ReadExplicitHeightLabelSeed(const TSharedPtr<FJsonObject> &Object,
                            const FLevelRuntimeLabelSeed &Seed) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FLevelRuntimeLabelExplicitHeightFields>({FLevelRuntimeLabelExplicitHeightFields(), Object}, JSON_REQUIRED_ATOMS(HeightOffset)),
      [Seed](const FLevelRuntimeLabelExplicitHeightFields &Fields) {
        return AssignExplicitHeight(Seed, Fields);
      });
}

func::Maybe<FLevelRuntimeLabelSeed>
ReadReferenceScaleLabelSeed(const TSharedPtr<FJsonObject> &Object,
                            const FLevelRuntimeLabelSeed &Seed) {
  return func::fmap(
      ReadScaleSeed(JsonValues::RequiredField(Object, "ReferenceScale")),
      [Seed](const FLevelRuntimeScaleSeed &ReferenceScale) {
        return AssignReferenceScale(Seed, ReferenceScale);
      });
}

const TArray<FLevelRuntimeLabelHeightDeclaration> &LabelHeightDeclarations() {
  static const TArray<FLevelRuntimeLabelHeightDeclaration> Declarations = {
      {ELevelRuntimeLabelHeightMode::Explicit, ReadExplicitHeightLabelSeed},
      {ELevelRuntimeLabelHeightMode::LabelForScale,
       ReadReferenceScaleLabelSeed},
      {ELevelRuntimeLabelHeightMode::AboveBlock, ReadReferenceScaleLabelSeed}};
  return Declarations;
}

func::Maybe<FLevelRuntimeLabelHeightDeclaration>
FindLabelHeightDeclaration(ELevelRuntimeLabelHeightMode Height) {
  return func::find_array<FLevelRuntimeLabelHeightDeclaration>(
      LabelHeightDeclarations(),
      [Height](const FLevelRuntimeLabelHeightDeclaration &Declaration) {
        return Declaration.Height == Height;
      });
}

func::Maybe<FLevelRuntimeLabelSeed>
CompleteLabelHeight(const TSharedPtr<FJsonObject> &Object,
                    const FLevelRuntimeLabelSeed &Seed) {
  return func::match(
      FindLabelHeightDeclaration(Seed.Height),
      [Object, Seed](const FLevelRuntimeLabelHeightDeclaration &Declaration) {
        return Declaration.Read(Object, Seed);
      },
      []() { return func::nothing<FLevelRuntimeLabelSeed>(); });
}

func::Maybe<FLevelRuntimeLabelSeed>
ReadLabelSeedFields(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeLabelSeed>({FLevelRuntimeLabelSeed(), Request.Object}, JSON_REQUIRED_ATOMS(Id, Text, Anchor, HeightMode,
                                          EastLots, NorthLots, WorldSizeScale));
}

} // namespace

func::Maybe<FLevelRuntimeLabelSeed>
LabelFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::mbind(
      ReadLabelSeedFields(Request),
      [Request](const FLevelRuntimeLabelSeed &Seed) {
        return CompleteLabelHeight(Request.Object, Seed);
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
