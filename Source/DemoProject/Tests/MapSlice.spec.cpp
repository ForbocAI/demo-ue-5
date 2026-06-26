#include "CoreMinimal.h"
#include "Features/Components/MapLayout.h"
#include "Features/Components/OrthoData.h"
#include "Features/Components/RetroStyle.h"
#include "Features/Components/TerrainData.h"
#include "Features/Entities/HorseRoutes.h"
#include "Features/Entities/NatureFeatures.h"
#include "Features/Entities/TownLandmarks.h"
#include "Features/Systems/MapActions.h"
#include "Features/Systems/MapFactories.h"
#include "Features/Systems/MapSelectors.h"
#include "Features/Systems/MapSlice.h"
#include "Misc/AutomationTest.h"

using namespace ForbocAI::Demo::Map;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FMapSliceDataBackedMap,
    "ForbocAI.Map.Slice.DataBackedMap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FMapSliceDataBackedMap::RunTest(const FString &Parameters) {
  FMapTerrainData TerrainData;
  FMapOrthoData OrthoData;

  TestTrue(TEXT("French Gulch heightmap CSV loads"),
           TerrainData.LoadFromContent());
  TestTrue(TEXT("French Gulch USGS ortho CSV loads"),
           OrthoData.LoadFromContent());
  TestEqual(TEXT("Terrain grid is 65x65"), TerrainData.GetGridSize(), 65);
  TestEqual(TEXT("Ortho grid is 65x65"), OrthoData.GetGridSize(), 65);
  TestTrue(TEXT("Terrain has relief"),
           TerrainData.GetMaxElevationMeters() >
               TerrainData.GetMinElevationMeters() + 250.0f);

  rtk::EnhancedStore<FMapState> Store =
      MapSlice::ConfigureStore();
  Store.dispatch(MapActions::TerrainLoaded()(
      MapFactories::TerrainLoadedPayload(
          {TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
           TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
           TerrainData.GetMaxElevationMeters()})));

  const TArray<FMapLandmark> Landmarks =
      TownLandmarks::Build1899LandmarkSeed(TerrainData);
  Store.dispatch(MapActions::LandmarksSeeded()(Landmarks));
  Store.dispatch(MapActions::PlayerSpawnAnchored()(
      MapFactories::SpawnPointPayload(
          {MapLayout::ToWorld(TerrainData, MapLayout::PlayerSpawnPoint()),
           MapLayout::PlayerSpawnRotation(),
           MapLayout::PlayerSpawnAnchorLabel()})));

  const FMapState &State = Store.getState();
  TestTrue(TEXT("RTK slice records terrain load"),
           MapSelectors::SelectTerrainLoaded(State));
  TestEqual(TEXT("RTK entity adapter stores seeded landmarks"),
            MapSelectors::SelectLandmarks(State).Num(),
            Landmarks.Num());

  const func::Maybe<FMapLandmark> PostOffice =
      MapSelectors::SelectLandmarkById(State, TEXT("post-office"));
  TestTrue(TEXT("Post office landmark is selectable"), PostOffice.hasValue);
  if (PostOffice.hasValue) {
    TestEqual(TEXT("Post office label"), PostOffice.value.Label,
              FString(TEXT("U.S. Post Office false-front")));
  }

  const FMapSpawnPointPayload Spawn =
      MapSelectors::SelectPlayerSpawn(State);
  TestEqual(TEXT("Player spawn anchor"), Spawn.AnchorLabel,
            MapLayout::PlayerSpawnAnchorLabel());
  TestTrue(TEXT("Player spawn is above terrain"),
           Spawn.Location.Z >
               TerrainData.GetTerrainZAtWorld(
                   MapLayout::PlayerSpawnPoint().EastWest,
                   MapLayout::PlayerSpawnPoint().NorthSouth));

  const TArray<FMapHorseRouteSeed> HorseRouteSeeds =
      HorseRoutes::Build1899HorseRouteSeed();
  TestEqual(TEXT("1899 horse routes are seeded"), HorseRouteSeeds.Num(), 3);
  for (const FMapHorseRouteSeed &HorseRoute : HorseRouteSeeds) {
    TestTrue(FString::Printf(TEXT("%s has a patrol loop"), *HorseRoute.Name),
             HorseRoute.PatrolRoute.Num() >= 3);
  }

  const TArray<FNatureFeatureSeed> NatureFeatureSeeds =
      NatureFeatures::BuildClearCreekNatureSeed();
  TestTrue(TEXT("Clear Creek natural environment features are seeded"),
           NatureFeatureSeeds.Num() >= 12);

  bool bHasWater = false;
  bool bHasRocks = false;
  bool bHasVegetation = false;
  bool bHasPCGMarker = false;
  bool bHasWaterMarker = false;
  for (const FNatureFeatureSeed &Feature : NatureFeatureSeeds) {
    bHasWater |= Feature.Kind == ENatureFeatureKind::Water;
    bHasRocks |= Feature.Kind == ENatureFeatureKind::Rock;
    bHasVegetation |=
        Feature.Kind == ENatureFeatureKind::TreeGrove ||
        Feature.Kind == ENatureFeatureKind::Shrub;
    bHasPCGMarker |= Feature.Kind == ENatureFeatureKind::PCGMarker;
    bHasWaterMarker |=
        Feature.Kind == ENatureFeatureKind::WaterSystemMarker;
  }
  TestTrue(TEXT("Nature seed includes water"), bHasWater);
  TestTrue(TEXT("Nature seed includes rocks"), bHasRocks);
  TestTrue(TEXT("Nature seed includes vegetation"), bHasVegetation);
  TestTrue(TEXT("Nature seed includes PCG marker"), bHasPCGMarker);
  TestTrue(TEXT("Nature seed includes Water System marker"), bHasWaterMarker);

  const FMapRetroRenderProfile &RetroProfile =
      RetroStyle::RuntimeProfile();
  TestEqual(TEXT("Retro profile is set to 3pm"),
            RetroProfile.TimeOfDayHour, 15.0f);
  TestEqual(TEXT("Retro profile disables anti-aliasing"),
            RetroProfile.AntiAliasingMethod, 0);
  TestTrue(TEXT("Retro profile keeps long readable vistas"),
           RetroProfile.ViewDistanceScale >= 1.5f);

  const TArray<FMapRetroTextureSpec> TextureCatalog =
      RetroStyle::TextureCatalog();
  TestTrue(TEXT("Retro texture catalog covers terrain, level, NPC, and horse surfaces"),
           TextureCatalog.Num() >= 13);

  return true;
}
