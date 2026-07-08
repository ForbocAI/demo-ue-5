#include "Features/Systems/Level/Layout/Scale/ScaleAdapters.h"

#include "Features/Components/Data/Json/Value/ValueAdapters.h"
#include "Features/Systems/Level/Layout/Enum/EnumAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

struct FWorldLocationFields {
  float X;
  float Y;
  float Z;
};

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Game::Level::Layout::FWorldLocationFields, X, Y, Z);

JSON_REQUIRED_FIELD_REGISTRY(ForbocAI::Game::Level::FLevelScaleSeed,
                             WidthFeet, DepthFeet, HeightFeet, FrontageFeet,
                             Stories, LengthLots);

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

struct FLevelScaleFieldDeclaration {
  ELevelScaleMode Mode;
  TArray<const char *> FieldAtoms;

  FLevelScaleFieldDeclaration() = default;

  FLevelScaleFieldDeclaration(
      ELevelScaleMode InMode,
      std::initializer_list<const char *> InFieldAtoms)
      : Mode(InMode), FieldAtoms(InFieldAtoms) {}
};

FLevelScaleSeed ScaleSeedWithMode(ELevelScaleMode Mode) {
  FLevelScaleSeed Seed{};
  Seed.Mode = Mode;
  return Seed;
}

FVector WorldLocationFromFields(const FWorldLocationFields &Fields) {
  return FVector(Fields.X, Fields.Y, Fields.Z);
}

const TArray<FLevelScaleFieldDeclaration> &
LevelScaleFieldDeclarations() {
  static const TArray<FLevelScaleFieldDeclaration> Declarations = {
      {ELevelScaleMode::Building,
       JSON_REQUIRED_ATOMS(FrontageFeet, DepthFeet, Stories)},
      {ELevelScaleMode::LongFeature,
       JSON_REQUIRED_ATOMS(WidthFeet, LengthLots, HeightFeet)},
      {ELevelScaleMode::Pad,
       JSON_REQUIRED_ATOMS(WidthFeet, DepthFeet, HeightFeet)}};
  return Declarations;
}

func::Maybe<FLevelScaleFieldDeclaration>
FindScaleFieldDeclaration(ELevelScaleMode Mode) {
  return func::find_array<FLevelScaleFieldDeclaration>(
      LevelScaleFieldDeclarations(),
      [Mode](const FLevelScaleFieldDeclaration &Declaration) {
        return Declaration.Mode == Mode;
      });
}

func::Maybe<FLevelScaleSeed>
ReadScaleSeedForMode(const TSharedPtr<FJsonObject> &Object,
                     ELevelScaleMode Mode) {
  return func::match(
      FindScaleFieldDeclaration(Mode),
      [Object](const FLevelScaleFieldDeclaration &Declaration) {
        return JsonValues::ReadRequiredFields<FLevelScaleSeed>({ScaleSeedWithMode(Declaration.Mode), Object}, Declaration.FieldAtoms);
      },
      []() { return func::nothing<FLevelScaleSeed>(); });
}

} // namespace

func::Maybe<FVector>
WorldLocationFromJson(const FLevelJsonObjectRequest &Request) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FWorldLocationFields>({FWorldLocationFields(), Request.Object}, JSON_REQUIRED_ATOMS(X, Y, Z)),
      WorldLocationFromFields);
}

func::Maybe<FLevelScaleSeed>
ScaleFromJson(const FLevelJsonObjectRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredField<FString>(Request.Object, "Mode"),
      [Request](const FString &ModeText) {
        const FLevelEnumTextRequest ModeRequest{
            ModeText, JsonValues::RequiredFieldName("Mode")};
        return func::mbind(
            ParseScaleMode(ModeRequest),
            [Request](ELevelScaleMode ParsedMode) {
              return ReadScaleSeedForMode(Request.Object, ParsedMode);
            });
      });
}

func::Maybe<FLevelScaleSeed>
ReadScaleSeed(const ForbocAI::Game::Data::FJsonFieldRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredObject(Request),
      [](const TSharedPtr<FJsonObject> &ScaleObject) {
        return ScaleFromJson({ScaleObject});
      });
}

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
