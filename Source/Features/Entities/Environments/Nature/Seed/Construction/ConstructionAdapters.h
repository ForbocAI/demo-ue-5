#pragma once

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/Json/JsonAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/Json/SeedJsonTypes.h"
#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

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

  /** User Story: As a nature seed construction consumer, I need to invoke fscale projector declaration through a stable signature so the nature seed construction workflow remains explicit and composable. @fn FScaleProjectorDeclaration() = default */
  FScaleProjectorDeclaration() = default;

  /** User Story: As a nature seed construction consumer, I need to invoke fscale projector declaration through a stable signature so the nature seed construction workflow remains explicit and composable. @fn FScaleProjectorDeclaration(ENatureScaleMode InMode, FScaleFieldsProjector InProject) */
  FScaleProjectorDeclaration(ENatureScaleMode InMode,
                             FScaleFieldsProjector InProject)
      : Mode(InMode), Project(InProject) {}
};

struct FFeatureBuildRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FNatureFeatureSource Fields;
};

namespace {

/** User Story: As a nature seed construction consumer, I need to invoke feature lots through a stable signature so the nature seed construction workflow remains explicit and composable. @fn FLevelLocalPoint FeatureLots(const FFeatureLotsRequest &Request) */
FLevelLocalPoint FeatureLots(const FFeatureLotsRequest &Request) {
  return LevelLayoutAdapters::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutAdapters::FromPostOfficeLots(
           {Request.Geometry, Request.EastLots, Request.NorthLots, 0.0f}),
      Request.Scale,
      LevelLayoutAdapters::RoadSurfaceClearance(Request.Geometry)});
}

/** User Story: As a nature seed construction consumer, I need to invoke nature kind from json through a stable signature so the nature seed construction workflow remains explicit and composable. @fn EFeatureKind NatureKindFromJson(const FString &Kind) */
EFeatureKind NatureKindFromJson(const FString &Kind) {
  return JsonAdapters::RequireRegisteredTextValue<EFeatureKind>(
      Kind, TEXT("nature feature kind"));
}

/** User Story: As a nature seed construction consumer, I need to invoke nature scale mode from json through a stable signature so the nature seed construction workflow remains explicit and composable. @fn ENatureScaleMode NatureScaleModeFromJson(const FString &Mode) */
ENatureScaleMode NatureScaleModeFromJson(const FString &Mode) {
  return JsonAdapters::RequireRegisteredTextValue<ENatureScaleMode>(
      Mode, TEXT("nature feature scale mode"));
}

/** User Story: As a nature seed construction consumer, I need to invoke pad scale from fields through a stable signature so the nature seed construction workflow remains explicit and composable. @fn FVector PadScaleFromFields(const FScaleFieldsRequest &Request) */
FVector PadScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNaturePadScaleSource Fields =
      JsonAdapters::ReadSettingsFields<FNaturePadScaleSource>(
          Request.Object, JSON_SETTINGS_ATOMS(WidthFeet, DepthFeet,
                                              HeightFeet));
  return LevelLayoutAdapters::PadScaleFromFeet(
      {Request.Geometry, Fields.WidthFeet, Fields.DepthFeet,
       Fields.HeightFeet});
}

/** User Story: As a nature seed construction consumer, I need to invoke long feature scale from fields through a stable signature so the nature seed construction workflow remains explicit and composable. @fn FVector LongFeatureScaleFromFields(const FScaleFieldsRequest &Request) */
FVector LongFeatureScaleFromFields(const FScaleFieldsRequest &Request) {
  const FNatureLongFeatureScaleSource Fields =
      JsonAdapters::ReadSettingsFields<FNatureLongFeatureScaleSource>(
          Request.Object, JSON_SETTINGS_ATOMS(WidthFeet, LengthLots,
                                              HeightFeet));
  return LevelLayoutAdapters::LongFeatureScale(
      {Request.Geometry, Fields.WidthFeet, Fields.LengthLots,
       Fields.HeightFeet});
}

/** User Story: As a nature seed construction consumer, I need to invoke scale projector declarations through a stable signature so the nature seed construction workflow remains explicit and composable. @fn const TArray<FScaleProjectorDeclaration> &ScaleProjectorDeclarations() */
const TArray<FScaleProjectorDeclaration> &ScaleProjectorDeclarations() {
  static const TArray<FScaleProjectorDeclaration> Declarations = {
      {ENatureScaleMode::LongFeature, LongFeatureScaleFromFields},
      {ENatureScaleMode::Pad, PadScaleFromFields}};
  return Declarations;
}

/** User Story: As a nature seed construction consumer, I need to invoke find scale projector through a stable signature so the nature seed construction workflow remains explicit and composable. @fn func::Maybe<FScaleProjectorDeclaration> FindScaleProjector(ENatureScaleMode Mode) */
func::Maybe<FScaleProjectorDeclaration>
FindScaleProjector(ENatureScaleMode Mode) {
  return func::find_array<FScaleProjectorDeclaration>(
      ScaleProjectorDeclarations(),
      [Mode](const FScaleProjectorDeclaration &Declaration) {
        return Declaration.Mode == Mode;
      });
}

/** User Story: As a nature seed construction consumer, I need to invoke scale object from fields through a stable signature so the nature seed construction workflow remains explicit and composable. @fn FVector ScaleObjectFromFields(const FScaleFieldsRequest &Request) */
FVector ScaleObjectFromFields(const FScaleFieldsRequest &Request) {
  const FNatureScaleModeSource Fields =
      JsonAdapters::ReadSettingsFields<FNatureScaleModeSource>(
          Request.Object, JSON_SETTINGS_ATOMS(Mode));
  const func::Maybe<FScaleProjectorDeclaration> Projector =
      FindScaleProjector(NatureScaleModeFromJson(Fields.Mode));
  checkf(Projector.hasValue, TEXT("Invalid nature feature scale mode"));
  return Projector.value.Project(Request);
}

/** User Story: As a nature seed construction consumer, I need to invoke scale from fields through a stable signature so the nature seed construction workflow remains explicit and composable. @fn FVector ScaleFromFields(const FFeatureBuildRequest &Request) */
FVector ScaleFromFields(const FFeatureBuildRequest &Request) {
  return ScaleObjectFromFields({Request.Geometry, Request.Fields.Scale});
}

/** User Story: As a nature seed construction consumer, I need to invoke feature from fields through a stable signature so the nature seed construction workflow remains explicit and composable. @fn FFeatureSeed FeatureFromFields(const FFeatureBuildRequest &Request) */
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

/** User Story: As a nature seed construction consumer, I need to invoke build nature seed through a stable signature so the nature seed construction workflow remains explicit and composable. @fn TArray<FFeatureSeed> BuildNatureSeed( const FBuildRequest &Request) */
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
