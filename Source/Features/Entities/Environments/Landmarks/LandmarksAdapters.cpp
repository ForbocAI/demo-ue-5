#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarksAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

namespace {

struct FLandmarkFromJsonRequest {
  const FLevelTerrainData *TerrainData = nullptr;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  TSharedPtr<FJsonObject> LandmarkObject;
};

FLandmark LandmarkFromJson(const FLandmarkFromJsonRequest &Request) {
  const JsonAdapters::FJsonStringReader String =
      JsonAdapters::StringIn(Request.LandmarkObject);
  const JsonAdapters::FJsonFloatReader Float =
      JsonAdapters::FloatIn(Request.LandmarkObject);
  const FVector Scale = LevelLayoutSlice::BuildingScaleFromFeet(
      {Request.Geometry, Float(TEXT("frontage_feet")),
       Float(TEXT("depth_feet")), Float(TEXT("stories"))});
  const FLevelLocalPoint Local = LevelLayoutSlice::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutSlice::FromPostOfficeLots(
           {Request.Geometry, Float(TEXT("east_lots")), Float(TEXT("north_lots")),
            0.0f}),
       Scale, LevelLayoutSlice::BuildingFoundationHeight(Request.Geometry)});

  return LandmarkFactories::Landmark(
      {String(TEXT("id")), String(TEXT("label")), ELandmarkKind::Building,
       LevelLayoutSlice::ToWorld({*Request.TerrainData, Local}), Scale});
}

} // namespace

TArray<FLandmark>
BuildLandmarkSeed(const FLandmarkSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  const JsonAdapters::FJsonArrayReader Array = JsonAdapters::ArrayIn(Root);
  return JsonAdapters::MapJsonValues<FLandmark>(
      Array(TEXT("landmarks")),
      [&Request](const TSharedPtr<FJsonObject> &LandmarkObject) {
        return LandmarkFromJson(
            {&Request.TerrainData, Request.Geometry, LandmarkObject});
      });
}

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
