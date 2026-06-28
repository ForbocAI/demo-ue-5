#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"

#include "Features/Components/Data/DataAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarksAdapters {

namespace DataAdapters = ForbocAI::Demo::Data::DataAdapters;

namespace {

constexpr const TCHAR *LandmarkDataPath =
    TEXT("Data/french_gulch_landmarks.json");

struct FLandmarkFromJsonRequest {
  const FLevelTerrainData *TerrainData = nullptr;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  TSharedPtr<FJsonObject> LandmarkObject;
};

FLandmark LandmarkFromJson(const FLandmarkFromJsonRequest &Request) {
  const FVector Scale = LevelLayoutSlice::BuildingScaleFromFeet(
      {Request.Geometry,
       DataAdapters::ReadFloat({Request.LandmarkObject, TEXT("frontage_feet")}),
       DataAdapters::ReadFloat({Request.LandmarkObject, TEXT("depth_feet")}),
       DataAdapters::ReadFloat({Request.LandmarkObject, TEXT("stories")})});
  const FLevelLocalPoint Local = LevelLayoutSlice::CenteredOnGround(
      {Request.Geometry,
       LevelLayoutSlice::FromPostOfficeLots(
           {Request.Geometry,
            DataAdapters::ReadFloat({Request.LandmarkObject, TEXT("east_lots")}),
            DataAdapters::ReadFloat({Request.LandmarkObject, TEXT("north_lots")}),
            0.0f}),
       Scale, LevelLayoutSlice::BuildingFoundationHeight(Request.Geometry)});

  return LandmarkFactories::Landmark(
      {DataAdapters::ReadString({Request.LandmarkObject, TEXT("id")}),
       DataAdapters::ReadString({Request.LandmarkObject, TEXT("label")}),
       ELandmarkKind::Building,
       LevelLayoutSlice::ToWorld({*Request.TerrainData, Local}), Scale});
}

} // namespace

TArray<FLandmark>
Build1899LandmarkSeed(const FLandmarkSeedBuildRequest &Request) {
  return func::match(
      DataAdapters::LoadObjectFromContent({LandmarkDataPath}),
      [&Request](const TSharedPtr<FJsonObject> &Root) {
        return DataAdapters::MapJsonValues<FLandmark>(
            {DataAdapters::ReadArray({Root, TEXT("landmarks")}),
             [&Request](const TSharedPtr<FJsonObject> &LandmarkObject) {
               return LandmarkFromJson(
                   {&Request.TerrainData, Request.Geometry, LandmarkObject});
             }});
      },
      []() { return TArray<FLandmark>(); });
}

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
