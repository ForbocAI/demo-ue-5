#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarksAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

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

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Demo::Level;
namespace LandmarkTypes = ForbocAI::Demo::Level::LandmarksAdapters;

using ELandmarkKind = LevelTypes::ELandmarkKind;
using FLandmarkFields = LandmarkTypes::FLandmarkFields;

template <> struct TJsonTextValueRegistry<ELandmarkKind> {
  static const TArray<TTextValueDeclaration<ELandmarkKind>> &Values() {
    static const TArray<TTextValueDeclaration<ELandmarkKind>>
        RegisteredValues = {{"building", ELandmarkKind::Building},
                            {"road", ELandmarkKind::Road},
                            {"creek", ELandmarkKind::Creek},
                            {"terrain_marker", ELandmarkKind::TerrainMarker},
                            {"mine", ELandmarkKind::Mine},
                            {"cemetery", ELandmarkKind::Cemetery},
                            {"park", ELandmarkKind::Park}};
    return RegisteredValues;
  }
};

JSON_SETTINGS_REGISTRY(FLandmarkFields, Id, Label, Kind, EastLots, NorthLots,
                       FrontageFeet, DepthFeet, Stories);

} // namespace JsonAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarksAdapters {
namespace {

struct FLandmarkBuildRequest {
  const FLevelTerrainData *TerrainData;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  FLandmarkFields Fields;
};

ELandmarkKind LandmarkKindFromJson(const FString &Kind) {
  return JsonAdapters::RequireRegisteredTextValue<ELandmarkKind>(
      Kind, TEXT("landmark kind"));
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
      JsonAdapters::ReadSettingsObjectArrayField<FLandmarkFields>(
          Root, "Landmarks",
          JSON_SETTINGS_ATOMS(Id, Label, Kind, EastLots, NorthLots,
                              FrontageFeet, DepthFeet, Stories)),
      [&Request](const FLandmarkFields &Fields) {
        return LandmarkFromFields(
            {&Request.TerrainData, Request.Geometry, Fields});
      });
}

} // namespace LandmarksAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
