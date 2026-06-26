#include "CoreMinimal.h"
#include "Features/Components/MapLayout.h"
#include "Features/Components/OrthoData.h"
#include "Features/Components/RetroStyle.h"
#include "Features/Components/TerrainData.h"
#include "Features/Entities/HorseRoutes.h"
#include "Features/Entities/NatureFeatures.h"
#include "Features/Entities/TownLandmarks.h"
#include "Features/Entities/TownspersonRoutes.h"
#include "Features/Systems/Horses/HorseSlice.h"
#include "Features/Systems/Landmarks/LandmarkSlice.h"
#include "Features/Systems/Nature/NatureSlice.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/Townspeople/TownspersonSlice.h"
#include "Misc/AutomationTest.h"

using namespace ForbocAI::Demo::Map;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FRuntimeStoreDataBackedMap,
    "ForbocAI.Runtime.Store.DataBackedMap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRuntimeStoreDataBackedMap::RunTest(const FString &Parameters) {
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

  rtk::EnhancedStore<FRuntimeState> Store =
      RuntimeStore::ConfigureStore();
  Store.dispatch(TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
           TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
           TerrainData.GetMaxElevationMeters()})));

  const TArray<FLandmark> Landmarks =
      TownLandmarks::Build1899LandmarkSeed(TerrainData);
  Store.dispatch(LandmarkActions::LandmarksSeeded()(Landmarks));
  Store.dispatch(SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {MapLayout::ToWorld(TerrainData, MapLayout::PlayerSpawnPoint()),
           MapLayout::PlayerSpawnRotation(),
           MapLayout::PlayerSpawnAnchorLabel()})));

  const TArray<FTownspersonSeed> TownspersonSeeds =
      TownspersonRoutes::Build1899TownspersonSeed();
  Store.dispatch(TownspersonActions::TownspeopleSeeded()(TownspersonSeeds));

  const TArray<FHorseRouteSeed> HorseRouteSeeds =
      HorseRoutes::Build1899HorseRouteSeed();
  Store.dispatch(HorseActions::HorsesSeeded()(HorseRouteSeeds));

  const TArray<FNatureFeatureSeed> NatureFeatureSeeds =
      NatureFeatures::BuildClearCreekNatureSeed();
  Store.dispatch(NatureActions::NatureSeeded()(NatureFeatureSeeds));

  const FRuntimeState &State = Store.getState();
  TestTrue(TEXT("RTK slice records terrain load"),
           RuntimeSelectors::SelectTerrainLoaded(State));
  TestEqual(TEXT("RTK entity adapter stores seeded landmarks"),
            RuntimeSelectors::SelectLandmarks(State).Num(),
            Landmarks.Num());

  const func::Maybe<FLandmark> PostOffice =
      RuntimeSelectors::SelectLandmarkById(State, TEXT("post-office"));
  TestTrue(TEXT("Post office landmark is selectable"), PostOffice.hasValue);
  if (PostOffice.hasValue) {
    TestEqual(TEXT("Post office label"), PostOffice.value.Label,
              FString(TEXT("U.S. Post Office false-front")));
  }

  const FSpawnPointPayload Spawn =
      RuntimeSelectors::SelectPlayerSpawn(State);
  TestEqual(TEXT("Player spawn anchor"), Spawn.AnchorLabel,
            MapLayout::PlayerSpawnAnchorLabel());
  TestTrue(TEXT("Player spawn is above terrain"),
           Spawn.Location.Z >
               TerrainData.GetTerrainZAtWorld(
                   MapLayout::PlayerSpawnPoint().EastWest,
                   MapLayout::PlayerSpawnPoint().NorthSouth));

  TestEqual(TEXT("1899 horse routes are seeded"), HorseRouteSeeds.Num(), 3);
  TestEqual(TEXT("RTK entity adapter stores seeded horses"),
            RuntimeSelectors::SelectHorses(State).Num(),
            HorseRouteSeeds.Num());
  for (const FHorseRouteSeed &HorseRoute : HorseRouteSeeds) {
    TestTrue(FString::Printf(TEXT("%s has a patrol loop"), *HorseRoute.Name),
             HorseRoute.PatrolRoute.Num() >= 3);
  }

  TestEqual(TEXT("RTK entity adapter stores seeded townspeople"),
            RuntimeSelectors::SelectTownspeople(State).Num(),
            TownspersonSeeds.Num());

  const int32 BotEntityCount =
      TownspersonSeeds.Num() + HorseRouteSeeds.Num();
  TestEqual(TEXT("RTK bot entity subdomain indexes townspeople and horses"),
            RuntimeSelectors::SelectBots(State).Num(), BotEntityCount);
  TestEqual(TEXT("RTK bot stats subdomain indexes townspeople and horses"),
            RuntimeSelectors::SelectBotStats(State).Num(), BotEntityCount);
  TestEqual(TEXT("RTK bot position subdomain indexes townspeople and horses"),
            RuntimeSelectors::SelectBotPositions(State).Num(), BotEntityCount);
  TestEqual(TEXT("RTK bot AI subdomain indexes townspeople and horses"),
            RuntimeSelectors::SelectBotAI(State).Num(), BotEntityCount);
  TestEqual(TEXT("RTK bot goal subdomain indexes townspeople and horses"),
            RuntimeSelectors::SelectBotGoals(State).Num(), BotEntityCount);

  const func::Maybe<FBotEntity> Clara =
      RuntimeSelectors::SelectBotById(State, TEXT("clara-bell"));
  TestTrue(TEXT("Townsperson bot entity is selectable"), Clara.hasValue);
  if (Clara.hasValue) {
    TestTrue(TEXT("Clara is a townsperson bot"),
             Clara.value.Kind == EBotEntityKind::Townsperson);
    TestTrue(TEXT("Clara is friendly"),
             Clara.value.Alignment == EBotAlignment::Friendly);
  }

  const func::Maybe<FBotStatsComponent> SorrelStats =
      RuntimeSelectors::SelectBotStatsById(State, TEXT("sorrel-at-livery"));
  TestTrue(TEXT("Horse bot stats are selectable"), SorrelStats.hasValue);
  if (SorrelStats.hasValue) {
    TestTrue(TEXT("Sorrel is not a mounted rider"),
             !SorrelStats.value.bMountedRider);
    TestTrue(TEXT("Sorrel has route movement speed"),
             SorrelStats.value.MoveSpeed > 0.0f);
  }

  const func::Maybe<FBotStrategicGoal> PostRoadGoal =
      RuntimeSelectors::SelectBotActiveGoalById(State,
                                                TEXT("post-road-rider"));
  TestTrue(TEXT("Horse bot active goal is selectable"),
           PostRoadGoal.hasValue);
  if (PostRoadGoal.hasValue) {
    TestTrue(TEXT("Post road rider has a patrol goal"),
             PostRoadGoal.value.Type == EBotGoalType::Patrol);
    TestTrue(TEXT("Post road rider goal targets a route location"),
             PostRoadGoal.value.bHasTargetLocation);
  }

  TestTrue(TEXT("Clear Creek natural environment features are seeded"),
           NatureFeatureSeeds.Num() >= 12);
  TestEqual(TEXT("RTK entity adapter stores seeded nature features"),
            RuntimeSelectors::SelectNatureFeatures(State).Num(),
            NatureFeatureSeeds.Num());

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
