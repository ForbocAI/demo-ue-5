#include "Features/Systems/Level/RuntimeLayout/Scale/Adapters.h"

#include "Features/Components/Data/Json/Value/Adapters.h"
#include "Features/Systems/Level/RuntimeLayout/Enum/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {

struct FWorldLocationFields {
  float X;
  float Y;
  float Z;
};

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Game::Level::RuntimeLayout::FWorldLocationFields, X, Y, Z);

JSON_REQUIRED_FIELD_REGISTRY(ForbocAI::Game::Level::FLevelRuntimeScaleSeed,
                             WidthFeet, DepthFeet, HeightFeet, FrontageFeet,
                             Stories, LengthLots);

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

struct FLevelRuntimeScaleFieldDeclaration {
  ELevelRuntimeScaleMode Mode;
  TArray<const char *> FieldAtoms;

  FLevelRuntimeScaleFieldDeclaration() = default;

  FLevelRuntimeScaleFieldDeclaration(
      ELevelRuntimeScaleMode InMode,
      std::initializer_list<const char *> InFieldAtoms)
      : Mode(InMode), FieldAtoms(InFieldAtoms) {}
};

FLevelRuntimeScaleSeed ScaleSeedWithMode(ELevelRuntimeScaleMode Mode) {
  FLevelRuntimeScaleSeed Seed{};
  Seed.Mode = Mode;
  return Seed;
}

FVector WorldLocationFromFields(const FWorldLocationFields &Fields) {
  return FVector(Fields.X, Fields.Y, Fields.Z);
}

const TArray<FLevelRuntimeScaleFieldDeclaration> &
LevelRuntimeScaleFieldDeclarations() {
  static const TArray<FLevelRuntimeScaleFieldDeclaration> Declarations = {
      {ELevelRuntimeScaleMode::Building,
       JSON_REQUIRED_ATOMS(FrontageFeet, DepthFeet, Stories)},
      {ELevelRuntimeScaleMode::LongFeature,
       JSON_REQUIRED_ATOMS(WidthFeet, LengthLots, HeightFeet)},
      {ELevelRuntimeScaleMode::Pad,
       JSON_REQUIRED_ATOMS(WidthFeet, DepthFeet, HeightFeet)}};
  return Declarations;
}

func::Maybe<FLevelRuntimeScaleFieldDeclaration>
FindScaleFieldDeclaration(ELevelRuntimeScaleMode Mode) {
  return func::find_array<FLevelRuntimeScaleFieldDeclaration>(
      LevelRuntimeScaleFieldDeclarations(),
      [Mode](const FLevelRuntimeScaleFieldDeclaration &Declaration) {
        return Declaration.Mode == Mode;
      });
}

func::Maybe<FLevelRuntimeScaleSeed>
ReadScaleSeedForMode(const TSharedPtr<FJsonObject> &Object,
                     ELevelRuntimeScaleMode Mode) {
  return func::match(
      FindScaleFieldDeclaration(Mode),
      [Object](const FLevelRuntimeScaleFieldDeclaration &Declaration) {
        return JsonValues::ReadRequiredFields<FLevelRuntimeScaleSeed>({ScaleSeedWithMode(Declaration.Mode), Object}, Declaration.FieldAtoms);
      },
      []() { return func::nothing<FLevelRuntimeScaleSeed>(); });
}

} // namespace

func::Maybe<FVector>
WorldLocationFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FWorldLocationFields>({FWorldLocationFields(), Request.Object}, JSON_REQUIRED_ATOMS(X, Y, Z)),
      WorldLocationFromFields);
}

func::Maybe<FLevelRuntimeScaleSeed>
ScaleFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredField<FString>(Request.Object, "Mode"),
      [Request](const FString &ModeText) {
        const FLevelRuntimeEnumTextRequest ModeRequest{
            ModeText, JsonValues::RequiredFieldName("Mode")};
        return func::mbind(
            ParseScaleMode(ModeRequest),
            [Request](ELevelRuntimeScaleMode ParsedMode) {
              return ReadScaleSeedForMode(Request.Object, ParsedMode);
            });
      });
}

func::Maybe<FLevelRuntimeScaleSeed>
ReadScaleSeed(const ForbocAI::Game::Data::FJsonFieldRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredObject(Request),
      [](const TSharedPtr<FJsonObject> &ScaleObject) {
        return ScaleFromJson({ScaleObject});
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
