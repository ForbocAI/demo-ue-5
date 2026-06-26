#include "CoreMinimal.h"
#include "FrenchGulch/Components/FrenchGulchLayout.h"
#include "FrenchGulch/Components/FrenchGulchOrthoData.h"
#include "FrenchGulch/Components/FrenchGulchTerrainData.h"
#include "FrenchGulch/Entities/FrenchGulchHorses.h"
#include "FrenchGulch/Entities/FrenchGulchNature.h"
#include "FrenchGulch/Entities/FrenchGulchTown.h"
#include "FrenchGulch/Systems/FrenchGulchSlice.h"
#include "Misc/AutomationTest.h"

using namespace ForbocAI::Demo::FrenchGulch;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FFrenchGulchSliceDataBackedMap,
    "ForbocAI.FrenchGulch.Slice.DataBackedMap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FFrenchGulchSliceDataBackedMap::RunTest(const FString &Parameters) {
  FFrenchGulchTerrainData TerrainData;
  FFrenchGulchOrthoData OrthoData;

  TestTrue(TEXT("French Gulch heightmap CSV loads"),
           TerrainData.LoadFromContent());
  TestTrue(TEXT("French Gulch USGS ortho CSV loads"),
           OrthoData.LoadFromContent());
  TestEqual(TEXT("Terrain grid is 65x65"), TerrainData.GetGridSize(), 65);
  TestEqual(TEXT("Ortho grid is 65x65"), OrthoData.GetGridSize(), 65);
  TestTrue(TEXT("Terrain has relief"),
           TerrainData.GetMaxElevationMeters() >
               TerrainData.GetMinElevationMeters() + 250.0f);

  rtk::EnhancedStore<FFrenchGulchMapState> Store =
      FrenchGulchSlice::ConfigureStore();
  Store.dispatch(FrenchGulchSlice::TerrainLoadedAction()(
      FrenchGulchFactory::TerrainLoadedPayload(
          TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
          TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
          TerrainData.GetMaxElevationMeters())));

  const TArray<FFrenchGulchLandmark> Landmarks =
      FrenchGulchTown::Build1899LandmarkSeed(TerrainData);
  Store.dispatch(FrenchGulchSlice::LandmarksSeededAction()(Landmarks));
  Store.dispatch(FrenchGulchSlice::PlayerSpawnAnchoredAction()(
      FrenchGulchFactory::SpawnPointPayload(
          FrenchGulchLayout::ToWorld(TerrainData,
                                     FrenchGulchLayout::PlayerSpawnPoint()),
          FrenchGulchLayout::PlayerSpawnRotation(),
          FrenchGulchLayout::PlayerSpawnAnchorLabel())));

  const FFrenchGulchMapState &State = Store.getState();
  TestTrue(TEXT("RTK slice records terrain load"),
           FrenchGulchSelectors::SelectTerrainLoaded(State));
  TestEqual(TEXT("RTK entity adapter stores seeded landmarks"),
            FrenchGulchSelectors::SelectLandmarks(State).Num(),
            Landmarks.Num());

  const func::Maybe<FFrenchGulchLandmark> PostOffice =
      FrenchGulchSelectors::SelectLandmarkById(State, TEXT("post-office"));
  TestTrue(TEXT("Post office landmark is selectable"), PostOffice.hasValue);
  if (PostOffice.hasValue) {
    TestEqual(TEXT("Post office label"), PostOffice.value.Label,
              FString(TEXT("U.S. Post Office false-front")));
  }

  const FFrenchGulchSpawnPointPayload Spawn =
      FrenchGulchSelectors::SelectPlayerSpawn(State);
  TestEqual(TEXT("Player spawn anchor"), Spawn.AnchorLabel,
            FrenchGulchLayout::PlayerSpawnAnchorLabel());
  TestTrue(TEXT("Player spawn is above terrain"),
           Spawn.Location.Z >
               TerrainData.GetTerrainZAtWorld(
                   FrenchGulchLayout::PlayerSpawnPoint().EastWest,
                   FrenchGulchLayout::PlayerSpawnPoint().NorthSouth));

  const TArray<FFrenchGulchHorseRouteSeed> HorseRoutes =
      FrenchGulchHorses::Build1899HorseRouteSeed();
  TestEqual(TEXT("1899 horse routes are seeded"), HorseRoutes.Num(), 3);
  for (const FFrenchGulchHorseRouteSeed &HorseRoute : HorseRoutes) {
    TestTrue(FString::Printf(TEXT("%s has a patrol loop"), *HorseRoute.Name),
             HorseRoute.PatrolRoute.Num() >= 3);
  }

  const TArray<FFrenchGulchNatureFeatureSeed> NatureFeatures =
      FrenchGulchNature::BuildClearCreekNatureSeed();
  TestTrue(TEXT("Clear Creek natural environment features are seeded"),
           NatureFeatures.Num() >= 8);

  bool bHasWater = false;
  bool bHasRocks = false;
  bool bHasVegetation = false;
  bool bHasPCGMarker = false;
  bool bHasWaterMarker = false;
  for (const FFrenchGulchNatureFeatureSeed &Feature : NatureFeatures) {
    bHasWater |= Feature.Kind == EFrenchGulchNatureFeatureKind::Water;
    bHasRocks |= Feature.Kind == EFrenchGulchNatureFeatureKind::Rock;
    bHasVegetation |=
        Feature.Kind == EFrenchGulchNatureFeatureKind::TreeGrove ||
        Feature.Kind == EFrenchGulchNatureFeatureKind::Shrub;
    bHasPCGMarker |= Feature.Kind == EFrenchGulchNatureFeatureKind::PCGMarker;
    bHasWaterMarker |=
        Feature.Kind == EFrenchGulchNatureFeatureKind::WaterSystemMarker;
  }
  TestTrue(TEXT("Nature seed includes water"), bHasWater);
  TestTrue(TEXT("Nature seed includes rocks"), bHasRocks);
  TestTrue(TEXT("Nature seed includes vegetation"), bHasVegetation);
  TestTrue(TEXT("Nature seed includes PCG marker"), bHasPCGMarker);
  TestTrue(TEXT("Nature seed includes Water System marker"), bHasWaterMarker);

  return true;
}
