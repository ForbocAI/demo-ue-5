#include "Features/Entities/TownLandmarks.h"

#include "Features/Components/MapLayout.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownLandmarks {
namespace {
void AddLandmark(TArray<FLandmark> &Landmarks,
                 const FMapTerrainData &TerrainData, const TCHAR *Id,
                 const TCHAR *Label, float EastLotsFromPostOffice,
                 float NorthLotsFromPostOffice, float FrontageFeet,
                 float DepthFeet, float Stories) {
  const FVector Scale = MapLayout::BuildingScaleFromFeet(
      FrontageFeet, DepthFeet, Stories);
  const FMapLocalPoint Local = MapLayout::CenteredOnGround(
      MapLayout::FromPostOfficeLots(EastLotsFromPostOffice,
                                            NorthLotsFromPostOffice),
      Scale, MapLayout::BuildingFoundationHeight());

  Landmarks.Add(LandmarkFactories::Landmark(
      {FString(Id), FString(Label), ELandmarkKind::Building,
       MapLayout::ToWorld(TerrainData, Local), Scale}));
}
} // namespace

TArray<FLandmark>
Build1899LandmarkSeed(const FMapTerrainData &TerrainData) {
  TArray<FLandmark> Landmarks;
  AddLandmark(Landmarks, TerrainData, TEXT("post-office"),
              TEXT("U.S. Post Office false-front"), 0.0f, 0.0f, 34.0f,
              19.0f, 1.5f);
  AddLandmark(Landmarks, TerrainData, TEXT("general-store"),
              TEXT("General store"), 1.0f, 0.45f, 29.0f, 18.0f, 1.4f);
  AddLandmark(Landmarks, TerrainData, TEXT("assay-office"),
              TEXT("Assay office"), 1.8f, -0.6f, 23.0f, 15.0f, 1.25f);
  AddLandmark(Landmarks, TerrainData, TEXT("hotel"),
              TEXT("French Gulch Hotel / boarding house"), 3.85f, 1.175f,
              43.0f, 24.0f, 1.85f);
  AddLandmark(Landmarks, TerrainData, TEXT("saloon"),
              TEXT("Saloon false-front"), 2.9f, -0.95f, 37.0f, 19.0f,
              1.65f);
  AddLandmark(Landmarks, TerrainData, TEXT("livery"),
              TEXT("Livery stable"), 4.8f, -1.275f, 44.0f, 27.0f, 1.3f);
  AddLandmark(Landmarks, TerrainData, TEXT("blacksmith"),
              TEXT("Blacksmith shop"), 5.65f, -0.275f, 27.0f, 18.0f,
              1.25f);
  AddLandmark(Landmarks, TerrainData, TEXT("sheriff"),
              TEXT("Sheriff office and jail"), 2.425f, 2.05f, 25.0f, 16.0f,
              1.25f);
  AddLandmark(Landmarks, TerrainData, TEXT("school-church"),
              TEXT("School / church"), 6.8f, 3.1f, 33.0f, 23.0f, 1.6f);
  AddLandmark(Landmarks, TerrainData, TEXT("fire-house"),
              TEXT("Volunteer fire house"), 3.3f, 4.25f, 31.0f, 20.0f,
              1.4f);
  return Landmarks;
}

} // namespace TownLandmarks
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
