#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"

#include <initializer_list>

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarksAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

namespace {

struct FLandmarkFields {
  FString Id;
  FString Label;
  FString Kind;
  float EastLots;
  float NorthLots;
  float FrontageFeet;
  float DepthFeet;
  float Stories;
};

struct FLandmarkBuildRequest {
  const FLevelTerrainData *TerrainData;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  FLandmarkFields Fields;
};

ELandmarkKind LandmarkKindFromJson(const FString &Kind) {
  const func::Maybe<ELandmarkKind> Parsed =
      JsonAdapters::ReadTextValue<ELandmarkKind>(
          Kind, {{"building", ELandmarkKind::Building},
                 {"road", ELandmarkKind::Road},
                 {"creek", ELandmarkKind::Creek},
                 {"terrain_marker", ELandmarkKind::TerrainMarker},
                 {"mine", ELandmarkKind::Mine},
                 {"cemetery", ELandmarkKind::Cemetery},
                 {"park", ELandmarkKind::Park}});
  checkf(Parsed.hasValue, TEXT("Invalid landmark kind: %s"), *Kind);
  return Parsed.value;
}

FLandmarkFields ReadLandmarkFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FLandmarkFields>(
      Object, JSON_SETTINGS_FIELDS(FLandmarkFields, Id, Label, Kind, EastLots,
                                   NorthLots, FrontageFeet, DepthFeet,
                                   Stories));
}

FLandmark LandmarkFromFields(const FLandmarkBuildRequest &Request) {
  const FVector Scale = LevelLayoutSlice::BuildingScaleFromFeet(
      {Request.Geometry, Request.Fields.FrontageFeet,
       Request.Fields.DepthFeet, Request.Fields.Stories});
  const FLevelLocalPoint Local = LevelLayoutSlice::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutSlice::FromPostOfficeLots(
           {Request.Geometry, Request.Fields.EastLots,
            Request.Fields.NorthLots, 0.0f}),
       Scale, LevelLayoutSlice::BuildingFoundationHeight(Request.Geometry)});

  return LandmarkFactories::Landmark(
      {Request.Fields.Id, Request.Fields.Label,
       LandmarkKindFromJson(Request.Fields.Kind),
       LevelLayoutSlice::ToWorld({*Request.TerrainData, Local}), Scale});
}

} // namespace

TArray<FLandmark>
BuildLandmarkSeed(const FLandmarkSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  return func::map_array<FLandmarkFields, FLandmark>(
      JsonAdapters::ReadObjectArrayField<FLandmarkFields>(
          Root, "Landmarks", ReadLandmarkFields),
      [&Request](const FLandmarkFields &Fields) {
        return LandmarkFromFields(
            {&Request.TerrainData, Request.Geometry, Fields});
      });
}

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
