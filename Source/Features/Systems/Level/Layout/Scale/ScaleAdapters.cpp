#include "Features/Systems/Level/Layout/Scale/ScaleAdapters.h"

#include "Features/Components/Data/Json/Value/JsonValueAdapters.h"
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

  /** User Story: As a level layout scale consumer, I need to invoke flevel scale field declaration through a stable signature so the level layout scale workflow remains explicit and composable. @fn FLevelScaleFieldDeclaration() = default */
  FLevelScaleFieldDeclaration() = default;

  /** User Story: As a level layout scale consumer, I need to invoke flevel scale field declaration through a stable signature so the level layout scale workflow remains explicit and composable. @fn FLevelScaleFieldDeclaration( EScaleMode InMode, std::initializer_list<const char *> InFieldAtoms) */
  FLevelScaleFieldDeclaration(
      EScaleMode InMode,
      std::initializer_list<const char *> InFieldAtoms)
      : Mode(InMode), FieldAtoms(InFieldAtoms) {}
};

/** User Story: As a level layout scale consumer, I need to invoke scale seed with mode through a stable signature so the level layout scale workflow remains explicit and composable. @fn FScaleSeed ScaleSeedWithMode(EScaleMode Mode) */
FScaleSeed ScaleSeedWithMode(EScaleMode Mode) {
  FScaleSeed Seed{};
  Seed.Mode = Mode;
  return Seed;
}

/** User Story: As a level layout scale consumer, I need to invoke world location from fields through a stable signature so the level layout scale workflow remains explicit and composable. @fn FVector WorldLocationFromFields(const FWorldLocationSource &Fields) */
FVector WorldLocationFromFields(const FWorldLocationSource &Fields) {
  return FVector(Fields.X, Fields.Y, Fields.Z);
}

/** User Story: As a level layout scale consumer, I need to invoke level scale field declarations through a stable signature so the level layout scale workflow remains explicit and composable. @fn const TArray<FLevelScaleFieldDeclaration> & LevelScaleFieldDeclarations() */
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

/** User Story: As a level layout scale consumer, I need to invoke find scale field declaration through a stable signature so the level layout scale workflow remains explicit and composable. @fn func::Maybe<FLevelScaleFieldDeclaration> FindScaleFieldDeclaration(EScaleMode Mode) */
func::Maybe<FLevelScaleFieldDeclaration>
FindScaleFieldDeclaration(EScaleMode Mode) {
  return func::find_array<FLevelScaleFieldDeclaration>(
      LevelScaleFieldDeclarations(),
      [Mode](const FLevelScaleFieldDeclaration &Declaration) {
        return Declaration.Mode == Mode;
      });
}

/** User Story: As a level layout scale consumer, I need to invoke read scale seed for mode through a stable signature so the level layout scale workflow remains explicit and composable. @fn func::Maybe<FScaleSeed> ReadScaleSeedForMode(const TSharedPtr<FJsonObject> &Object, EScaleMode Mode) */
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

/** User Story: As a level layout scale consumer, I need to invoke world location from json through a stable signature so the level layout scale workflow remains explicit and composable. @fn func::Maybe<FVector> WorldLocationFromJson(const FLevelJsonObjectRequest &Request) */
func::Maybe<FVector>
WorldLocationFromJson(const FLevelJsonObjectRequest &Request) {
  return func::fmap(
      JsonValues::ReadRequiredFields<FWorldLocationSource>({FWorldLocationSource(), Request.Object}, JSON_REQUIRED_ATOMS(X, Y, Z)),
      WorldLocationFromFields);
}

/** User Story: As a level layout scale consumer, I need to invoke scale from json through a stable signature so the level layout scale workflow remains explicit and composable. @fn func::Maybe<FScaleSeed> ScaleFromJson(const FLevelJsonObjectRequest &Request) */
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

/** User Story: As a level layout scale consumer, I need to invoke read scale seed through a stable signature so the level layout scale workflow remains explicit and composable. @fn func::Maybe<FScaleSeed> ReadScaleSeed(const ForbocAI::Game::Data::FFieldRequest &Request) */
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
