#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

#include <initializer_list>

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

namespace {

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
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
  FVector Scale;
};

struct FScaleFieldsRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  TSharedPtr<FJsonObject> Object;
};

typedef FVector (*FScaleFieldsProjector)(const FScaleFieldsRequest &);

struct FScaleProjectorDeclaration {
  ENatureScaleMode Mode;
  FScaleFieldsProjector Project;

  FScaleProjectorDeclaration(ENatureScaleMode InMode,
                             FScaleFieldsProjector InProject)
      : Mode(InMode), Project(InProject) {}
};

struct FFeatureBuildRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  FNatureFeatureFields Fields;
};

FLevelLocalPoint FeatureLots(const FFeatureLotsRequest &Request) {
  return LevelLayoutSlice::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutSlice::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
      Request.Scale,
      LevelLayoutSlice::RoadSurfaceClearance(Request.Geometry)});
}

ENatureFeatureKind NatureKindFromJson(const FString &Kind) {
  const func::Maybe<ENatureFeatureKind> Parsed =
      JsonAdapters::ReadTextValue<ENatureFeatureKind>(
          Kind, {{"water", ENatureFeatureKind::Water},
                 {"rock", ENatureFeatureKind::Rock},
                 {"tree_grove", ENatureFeatureKind::TreeGrove},
                 {"shrub", ENatureFeatureKind::Shrub},
                 {"water_system_marker",
                  ENatureFeatureKind::WaterSystemMarker},
                 {"pcg_marker", ENatureFeatureKind::PCGMarker}});
  checkf(Parsed.hasValue, TEXT("Invalid nature feature kind: %s"), *Kind);
  return Parsed.value;
}

ENatureScaleMode NatureScaleModeFromJson(const FString &Mode) {
  const func::Maybe<ENatureScaleMode> Parsed =
      JsonAdapters::ReadTextValue<ENatureScaleMode>(
          Mode, {{"long_feature", ENatureScaleMode::LongFeature},
                 {"pad", ENatureScaleMode::Pad},
                 {"pad_feet", ENatureScaleMode::Pad}});
  checkf(Parsed.hasValue, TEXT("Invalid nature feature scale mode: %s"),
         *Mode);
  return Parsed.value;
}

FNatureScaleModeFields
ReadScaleModeFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FNatureScaleModeFields>(
      Object, JSON_SETTINGS_FIELDS(FNatureScaleModeFields, Mode));
}

FNaturePadScaleFields
ReadPadScaleFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FNaturePadScaleFields>(
      Object, JSON_SETTINGS_FIELDS(FNaturePadScaleFields, WidthFeet,
                                   DepthFeet, HeightFeet));
}

FNatureLongFeatureScaleFields
ReadLongFeatureScaleFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FNatureLongFeatureScaleFields>(
      Object, JSON_SETTINGS_FIELDS(FNatureLongFeatureScaleFields, WidthFeet,
                                   LengthLots, HeightFeet));
}

FNatureFeatureFields
ReadFeatureFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FNatureFeatureFields>(
      Object, JSON_SETTINGS_FIELDS(FNatureFeatureFields, Id, Name, Kind,
                                   EastLots, NorthLots, Scale));
}

FVector PadScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNaturePadScaleFields Fields = ReadPadScaleFields(Request.Object);
  return LevelLayoutSlice::PadScaleFromFeet(
      {Request.Geometry, Fields.WidthFeet, Fields.DepthFeet,
       Fields.HeightFeet});
}

FVector LongFeatureScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNatureLongFeatureScaleFields Fields =
      ReadLongFeatureScaleFields(Request.Object);
  return LevelLayoutSlice::LongFeatureScale(
      {Request.Geometry, Fields.WidthFeet, Fields.LengthLots,
       Fields.HeightFeet});
}

func::Maybe<FVector> ProjectScaleFields(
    const FScaleFieldsRequest &Request,
    std::initializer_list<FScaleProjectorDeclaration> Declarations) {
  const ENatureScaleMode Mode =
      NatureScaleModeFromJson(ReadScaleModeFields(Request.Object).Mode);
  return func::fold_indexed(
      TArray<FScaleProjectorDeclaration>(Declarations),
      static_cast<size_t>(Declarations.size()), func::nothing<FVector>(),
      [Mode, Request](const func::Maybe<FVector> &Current,
                      const FScaleProjectorDeclaration &Declaration) {
        return Current.hasValue || Declaration.Mode != Mode
                   ? Current
                   : func::just(Declaration.Project(Request));
      });
}

FVector ScaleObjectFromFields(const FScaleFieldsRequest &Request) {
  const func::Maybe<FVector> Parsed = ProjectScaleFields(
      Request, {{ENatureScaleMode::LongFeature, LongFeatureScaleFromFields},
                {ENatureScaleMode::Pad, PadScaleFromFields}});
  checkf(Parsed.hasValue, TEXT("Invalid nature feature scale mode"));
  return Parsed.value;
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
          Root, "Features", ReadFeatureFields),
      [&Request](const FNatureFeatureFields &Fields) {
        return FeatureFromFields({Request.Geometry, Fields});
      });
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
