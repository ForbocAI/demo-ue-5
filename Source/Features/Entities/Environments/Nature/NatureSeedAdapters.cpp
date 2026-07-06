#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

enum class ENatureScaleMode { LongFeature, Pad };

struct FNatureScaleModeFields {
  FString Mode;
};

struct FNaturePadScaleFields {
  float WidthFeet;
  float DepthFeet;
  float HeightFeet;
};

struct FNatureLongFeatureScaleFields {
  float WidthFeet;
  float LengthLots;
  float HeightFeet;
};

struct FNatureFeatureFields {
  FString Id;
  FString Name;
  FString Kind;
  float EastLots;
  float NorthLots;
  TSharedPtr<FJsonObject> Scale;
};

struct FFeatureLotsRequest {
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
  FVector Scale;
};

struct FScaleFieldsRequest {
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  TSharedPtr<FJsonObject> Object;
};

typedef FVector (*FScaleFieldsProjector)(const FScaleFieldsRequest &);

struct FScaleProjectorDeclaration {
  ENatureScaleMode Mode;
  FScaleFieldsProjector Project;

  FScaleProjectorDeclaration() = default;

  FScaleProjectorDeclaration(ENatureScaleMode InMode,
                             FScaleFieldsProjector InProject)
      : Mode(InMode), Project(InProject) {}
};

struct FFeatureBuildRequest {
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FNatureFeatureFields Fields;
};

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Game::Level;
namespace NatureTypes = ForbocAI::Game::Level::NatureAdapters;

using ENatureFeatureKind = LevelTypes::ENatureFeatureKind;
using ENatureScaleMode = NatureTypes::ENatureScaleMode;
using FNatureFeatureFields = NatureTypes::FNatureFeatureFields;
using FNatureLongFeatureScaleFields =
    NatureTypes::FNatureLongFeatureScaleFields;
using FNaturePadScaleFields = NatureTypes::FNaturePadScaleFields;
using FNatureScaleModeFields = NatureTypes::FNatureScaleModeFields;

template <>
struct TJsonTextValueRegistry<ENatureFeatureKind> {
  static const TArray<TTextValueDeclaration<ENatureFeatureKind>> &Values() {
    static const TArray<TTextValueDeclaration<ENatureFeatureKind>>
        RegisteredValues = {{"water", ENatureFeatureKind::Water},
                            {"rock", ENatureFeatureKind::Rock},
                            {"tree_grove", ENatureFeatureKind::TreeGrove},
                            {"shrub", ENatureFeatureKind::Shrub},
                            {"water_system_marker",
                             ENatureFeatureKind::WaterSystemMarker},
                            {"pcg_marker", ENatureFeatureKind::PCGMarker}};
    return RegisteredValues;
  }
};

template <>
struct TJsonTextValueRegistry<ENatureScaleMode> {
  static const TArray<TTextValueDeclaration<ENatureScaleMode>> &Values() {
    static const TArray<TTextValueDeclaration<ENatureScaleMode>>
        RegisteredValues = {{"long_feature", ENatureScaleMode::LongFeature},
                            {"pad", ENatureScaleMode::Pad},
                            {"pad_feet", ENatureScaleMode::Pad}};
    return RegisteredValues;
  }
};

JSON_SETTINGS_REGISTRY(FNatureScaleModeFields, Mode);
JSON_SETTINGS_REGISTRY(FNaturePadScaleFields, WidthFeet, DepthFeet, HeightFeet);
JSON_SETTINGS_REGISTRY(FNatureLongFeatureScaleFields, WidthFeet, LengthLots,
                       HeightFeet);
JSON_SETTINGS_REGISTRY(FNatureFeatureFields, Id, Name, Kind, EastLots,
                       NorthLots, Scale);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {
namespace {

FLevelLocalPoint FeatureLots(const FFeatureLotsRequest &Request) {
  return LevelLayoutSlice::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutSlice::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
      Request.Scale,
      LevelLayoutSlice::RoadSurfaceClearance(Request.Geometry)});
}

ENatureFeatureKind NatureKindFromJson(const FString &Kind) {
  return JsonAdapters::RequireRegisteredTextValue<ENatureFeatureKind>(
      Kind, TEXT("nature feature kind"));
}

ENatureScaleMode NatureScaleModeFromJson(const FString &Mode) {
  return JsonAdapters::RequireRegisteredTextValue<ENatureScaleMode>(
      Mode, TEXT("nature feature scale mode"));
}

FVector PadScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNaturePadScaleFields Fields =
      JsonAdapters::ReadSettingsFields<FNaturePadScaleFields>(
          Request.Object, JSON_SETTINGS_ATOMS(WidthFeet, DepthFeet,
                                              HeightFeet));
  return LevelLayoutSlice::PadScaleFromFeet(
      {Request.Geometry, Fields.WidthFeet, Fields.DepthFeet,
       Fields.HeightFeet});
}

FVector LongFeatureScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNatureLongFeatureScaleFields Fields =
      JsonAdapters::ReadSettingsFields<FNatureLongFeatureScaleFields>(
          Request.Object, JSON_SETTINGS_ATOMS(WidthFeet, LengthLots,
                                              HeightFeet));
  return LevelLayoutSlice::LongFeatureScale(
      {Request.Geometry, Fields.WidthFeet, Fields.LengthLots,
       Fields.HeightFeet});
}

const TArray<FScaleProjectorDeclaration> &ScaleProjectorDeclarations() {
  static const TArray<FScaleProjectorDeclaration> Declarations = {
      {ENatureScaleMode::LongFeature, LongFeatureScaleFromFields},
      {ENatureScaleMode::Pad, PadScaleFromFields}};
  return Declarations;
}

func::Maybe<FScaleProjectorDeclaration>
FindScaleProjector(ENatureScaleMode Mode) {
  return func::find_array<FScaleProjectorDeclaration>(
      ScaleProjectorDeclarations(),
      [Mode](const FScaleProjectorDeclaration &Declaration) {
        return Declaration.Mode == Mode;
      });
}

FVector ScaleObjectFromFields(const FScaleFieldsRequest &Request) {
  const FNatureScaleModeFields Fields =
      JsonAdapters::ReadSettingsFields<FNatureScaleModeFields>(
          Request.Object, JSON_SETTINGS_ATOMS(Mode));
  const func::Maybe<FScaleProjectorDeclaration> Projector =
      FindScaleProjector(NatureScaleModeFromJson(Fields.Mode));
  checkf(Projector.hasValue, TEXT("Invalid nature feature scale mode"));
  return Projector.value.Project(Request);
}

FVector ScaleFromFields(const FFeatureBuildRequest &Request) {
  return ScaleObjectFromFields({Request.Geometry, Request.Fields.Scale});
}

FNatureFeatureSeed FeatureFromFields(const FFeatureBuildRequest &Request) {
  const FVector Scale = ScaleFromFields(Request);
  FNatureFeatureSeed Seed;
  Seed.Id = Request.Fields.Id;
  Seed.Name = Request.Fields.Name;
  Seed.Kind = NatureKindFromJson(Request.Fields.Kind);
  Seed.Scale = Scale;
  Seed.Location = FeatureLots(
      {Request.Geometry, Request.Fields.EastLots, Request.Fields.NorthLots,
       Scale});
  return Seed;
}

} // namespace

TArray<FNatureFeatureSeed> BuildNatureSeed(
    const FNatureSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  return func::map_array<FNatureFeatureFields, FNatureFeatureSeed>(
      JsonAdapters::ReadObjectArrayField<FNatureFeatureFields>(
          {Root, "Features"},
          JsonAdapters::ReadSettingsWith<FNatureFeatureFields>(
              JSON_SETTINGS_ATOMS(Id, Name, Kind, EastLots, NorthLots,
                                  Scale))),
      [&Request](const FNatureFeatureFields &Fields) {
        return FeatureFromFields({Request.Geometry, Fields});
      });
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
