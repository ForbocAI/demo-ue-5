#include "Features/Systems/Level/Layout/Scale/ScaleAdapters.h"

#include "Features/Components/Data/Json/Value/ValueAdapters.h"
#include "Features/Systems/Level/Layout/Enum/EnumAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

struct FWorldLocationSource {
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
    ForbocAI::Game::Level::Layout::FWorldLocationSource, X, Y, Z);

JSON_REQUIRED_FIELD_REGISTRY(ForbocAI::Game::Level::FScaleSeed,
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
  EScaleMode Mode;
  TArray<const char *> FieldAtoms;

  FLevelScaleFieldDeclaration() = default;

  FLevelScaleFieldDeclaration(
      EScaleMode InMode,
      std::initializer_list<const char *> InFieldAtoms)
      : Mode(InMode), FieldAtoms(InFieldAtoms) {}
};

FScaleSeed ScaleSeedWithMode(EScaleMode Mode) {
  FScaleSeed Seed{};
  Seed.Mode = Mode;
  return Seed;
}

FVector WorldLocationFromFields(const FWorldLocationSource &Fields) {
  return FVector(Fields.X, Fields.Y, Fields.Z);
}

const TArray<FLevelScaleFieldDeclaration> &
LevelScaleFieldDeclarations() {
  static const TArray<FLevelScaleFieldDeclaration> Declarations = {
      {EScaleMode::Building,
       JSON_REQUIRED_ATOMS(FrontageFeet, DepthFeet, Stories)},
      {EScaleMode::LongFeature,
       JSON_REQUIRED_ATOMS(WidthFeet, LengthLots, HeightFeet)},
      {EScaleMode::Pad,
       JSON_REQUIRED_ATOMS(WidthFeet, DepthFeet, HeightFeet)}};
  return Declarations;
}

func::Maybe<FLevelScaleFieldDeclaration>
FindScaleFieldDeclaration(EScaleMode Mode) {
  return func::find_array<FLevelScaleFieldDeclaration>(
      LevelScaleFieldDeclarations(),
      [Mode](const FLevelScaleFieldDeclaration &Declaration) {
        return Declaration.Mode == Mode;
      });
}

func::Maybe<FScaleSeed>
ReadScaleSeedForMode(const TSharedPtr<FJsonObject> &Object,
                     EScaleMode Mode) {
  return func::match(
      FindScaleFieldDeclaration(Mode),
      [Object](const FLevelScaleFieldDeclaration &Declaration) {
        return JsonValues::ReadRequiredFields<FScaleSeed>({ScaleSeedWithMode(Declaration.Mode), Object}, Declaration.FieldAtoms);
      },
      []() { return func::nothing<FScaleSeed>(); });
}

} // namespace

func::Maybe<FVector>
WorldLocationFromJson(const FLevelJsonObjectRequest &Request) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FWorldLocationSource>({FWorldLocationSource(), Request.Object}, JSON_REQUIRED_ATOMS(X, Y, Z)),
      WorldLocationFromFields);
}

func::Maybe<FScaleSeed>
ScaleFromJson(const FLevelJsonObjectRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredField<FString>(Request.Object, "Mode"),
      [Request](const FString &ModeText) {
        const FLevelEnumTextRequest ModeRequest{
            ModeText, JsonValues::RequiredFieldName("Mode")};
        return func::mbind(
            ParseScaleMode(ModeRequest),
            [Request](EScaleMode ParsedMode) {
              return ReadScaleSeedForMode(Request.Object, ParsedMode);
            });
      });
}

func::Maybe<FScaleSeed>
ReadScaleSeed(const ForbocAI::Game::Data::FFieldRequest &Request) {
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
