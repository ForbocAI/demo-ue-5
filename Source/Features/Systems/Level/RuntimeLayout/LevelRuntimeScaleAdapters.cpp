#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeScaleAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeEnumAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {

struct FWorldLocationFields {
  float X;
  float Y;
  float Z;
};

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonValueAdapters {

JSON_REQUIRED_FIELD_REGISTRY(
    ForbocAI::Demo::Level::RuntimeLayout::FWorldLocationFields, X, Y, Z);

JSON_REQUIRED_FIELD_REGISTRY(ForbocAI::Demo::Level::FLevelRuntimeScaleSeed,
                             WidthFeet, DepthFeet, HeightFeet, FrontageFeet,
                             Stories, LengthLots);

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {
namespace {

namespace JsonValues = ForbocAI::Demo::Data::JsonValueAdapters;

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
        return JsonValues::ReadRequiredFields<FLevelRuntimeScaleSeed>(
            Object, Declaration.FieldAtoms,
            ScaleSeedWithMode(Declaration.Mode));
      },
      []() { return func::nothing<FLevelRuntimeScaleSeed>(); });
}

} // namespace

func::Maybe<FVector>
WorldLocationFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FWorldLocationFields>(
          Request.Object, JSON_REQUIRED_ATOMS(X, Y, Z)),
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
ReadScaleSeed(const ForbocAI::Demo::Data::FJsonFieldRequest &Request) {
  return func::mbind(
      JsonValues::ReadRequiredObject(Request),
      [](const TSharedPtr<FJsonObject> &ScaleObject) {
        return ScaleFromJson({ScaleObject});
      });
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
