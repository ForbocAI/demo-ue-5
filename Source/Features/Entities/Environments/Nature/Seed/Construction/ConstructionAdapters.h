#pragma once

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/Json/JsonAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/Json/SeedJsonTypes.h"
#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

struct FFeatureLotsRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
  FVector Scale;
};

struct FScaleFieldsRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
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
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FNatureFeatureSource Fields;
};

namespace {

FLevelLocalPoint FeatureLots(const FFeatureLotsRequest &Request) {
  return LevelLayoutAdapters::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutAdapters::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
      Request.Scale,
      LevelLayoutAdapters::RoadSurfaceClearance(Request.Geometry)});
}

EFeatureKind NatureKindFromJson(const FString &Kind) {
  return JsonAdapters::RequireRegisteredTextValue<EFeatureKind>(
      Kind, TEXT("nature feature kind"));
}

ENatureScaleMode NatureScaleModeFromJson(const FString &Mode) {
  return JsonAdapters::RequireRegisteredTextValue<ENatureScaleMode>(
      Mode, TEXT("nature feature scale mode"));
}

FVector PadScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNaturePadScaleSource Fields =
      JsonAdapters::ReadSettingsFields<FNaturePadScaleSource>(
          Request.Object, JSON_SETTINGS_ATOMS(WidthFeet, DepthFeet,
                                              HeightFeet));
  return LevelLayoutAdapters::PadScaleFromFeet(
      {Request.Geometry, Fields.WidthFeet, Fields.DepthFeet,
       Fields.HeightFeet});
}

FVector LongFeatureScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNatureLongFeatureScaleSource Fields =
      JsonAdapters::ReadSettingsFields<FNatureLongFeatureScaleSource>(
          Request.Object, JSON_SETTINGS_ATOMS(WidthFeet, LengthLots,
                                              HeightFeet));
  return LevelLayoutAdapters::LongFeatureScale(
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
  const FNatureScaleModeSource Fields =
      JsonAdapters::ReadSettingsFields<FNatureScaleModeSource>(
          Request.Object, JSON_SETTINGS_ATOMS(Mode));
  const func::Maybe<FScaleProjectorDeclaration> Projector =
      FindScaleProjector(NatureScaleModeFromJson(Fields.Mode));
  checkf(Projector.hasValue, TEXT("Invalid nature feature scale mode"));
  return Projector.value.Project(Request);
}

FVector ScaleFromFields(const FFeatureBuildRequest &Request) {
  return ScaleObjectFromFields({Request.Geometry, Request.Fields.Scale});
}

FFeatureSeed FeatureFromFields(const FFeatureBuildRequest &Request) {
  const FVector Scale = ScaleFromFields(Request);
  FFeatureSeed Seed;
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

TArray<FFeatureSeed> BuildNatureSeed(
    const FBuildRequest &Request) {
  return func::map_array<FNatureFeatureSource, FFeatureSeed>(
      JsonAdapters::MapJsonValues<FNatureFeatureSource>(
          JsonAdapters::LoadRequiredArrayFromContent(
              {Request.RelativeJsonPath}),
          JsonAdapters::ReadSettingsWith<FNatureFeatureSource>(
              JSON_SETTINGS_ATOMS(Id, Name, Kind, EastLots, NorthLots,
                                  Scale))),
      [&Request](const FNatureFeatureSource &Fields) {
        return FeatureFromFields({Request.Geometry, Fields});
      });
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
