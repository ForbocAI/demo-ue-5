#include "Tests/Store/StoreSpecSupport.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreDataBackedMap,
    "ForbocAI.Runtime.Store.DataBackedMap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/** User Story: As a tests store consumer, I need to invoke run test through a stable signature so the tests store workflow remains explicit and composable. @fn bool FStoreDataBackedMap::RunTest(const FString &Parameters) */
bool FStoreDataBackedMap::RunTest(const FString &Parameters) {
  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const FStoreSettings StoreAutomation = Settings.Automation.Store;
  FLabelCursor Labels = StoreLabels(
      StoreAutomation.Labels.DataBackedMapLabels);
  const ForbocAI::Game::Data::FSourceSettings Sources =
      Settings.LevelTerrainSources;
  const ForbocAI::Game::Data::FDataSourceSettings DataSources =
      Settings.LevelDataSources;
  const ForbocAI::Game::Data::FValidationSettings Validation =
      Settings.Validation;
  const ForbocAI::Game::Data::FCsvSettings Csv =
      Settings.LevelCsv;
  const ForbocAI::Game::Data::FGeometrySettings Geometry =
      Settings.LevelGeometry;
  FLevelTerrainData TerrainData;
  FLevelOrthoData OrthoData;

  TestTrue(Labels.Next(),
           TerrainData.LoadFromContent({Sources, Csv, Geometry}));
  TestTrue(Labels.Next(),
           OrthoData.LoadFromContent({Sources, Csv}));
  TestEqual(Labels.Next(),
            TerrainData.GetGridSize(), Validation.TerrainGridSize);
  TestEqual(Labels.Next(),
            OrthoData.GetGridSize(), Validation.OrthoGridSize);
  TestTrue(Labels.Next(),
           TerrainData.GetMaxElevationMeters() >
               TerrainData.GetMinElevationMeters() +
                   Validation.TerrainMinReliefMeters);

  const float ActorWorldUnitsPerFoot =
      LevelLayoutAdapters::ActorWorldUnitsFromFeet(
          {Geometry,
           Geometry.ActorReferenceFeetAcross /
               Geometry.ActorReferenceFeetAcross});
  const float BlockWorldUnitsPerFoot =
      Geometry.BlockScalePerFoot * Geometry.CubeMeshSize;
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(BlockWorldUnitsPerFoot,
                                ActorWorldUnitsPerFoot));
  const float StoryWorldUnits =
      Geometry.HeightScalePerStory * Geometry.CubeMeshSize;
  const float CharacterHeightFeet =
      Settings.TownspersonPresentation.Geometry.CharacterHeightFeet;
  TestTrue(Labels.Next(),
           StoryWorldUnits >
               LevelLayoutAdapters::ActorWorldUnitsFromFeet(
                   {Geometry, CharacterHeightFeet}));

  const FLayoutSeed Layout =
      LevelAdapters::LoadLayoutSeed(DataSources);
  TestTrue(Labels.Next(),
           !Layout.Town.Labels.IsEmpty());
  check(!Layout.Town.Labels.IsEmpty());
  TestEqual(Labels.Next(),
            static_cast<int32>(
                Layout.Town.Labels[Layout.Town.Labels.Num() -
                                         Layout.Town.Labels.Num()]
                    .Height),
            static_cast<int32>(ELabelHeightMode::Explicit));
  TestTrue(Labels.Next(),
           !Layout.OverlayLabels.IsEmpty());
  TestTrue(Labels.Next(),
           Layout.Terrain.Blocks.Num() >
               Layout.OverlayLabels.Num());
  TestTrue(Labels.Next(),
           Layout.Town.Blocks.Num() > Layout.Town.Labels.Num());

  rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  EnhancedStoreValue.dispatch(TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
           TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
           TerrainData.GetMaxElevationMeters()})));

  const TArray<FLandmark> Landmarks =
      LandmarksAdapters::BuildLandmarkSeed(
          {DataSources.Entities.LandmarksJsonPath, TerrainData, Geometry});
  const FLandmark &FirstLandmark = RequiredFirstLandmark(Landmarks);
  EnhancedStoreValue.dispatch(LandmarkActions::LandmarksSeeded()(Landmarks));
  EnhancedStoreValue.dispatch(SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {LevelLayoutAdapters::ToWorld(
               {TerrainData, LevelLayoutAdapters::PlayerSpawnPoint(Geometry)}),
           LevelLayoutAdapters::PlayerSpawnRotation(Geometry),
           LevelLayoutAdapters::PlayerSpawnAnchorLabel(Geometry)})));

  const TArray<FTownspersonSeed> TownspersonSeeds =
      BotsAdapters::BuildTownspersonSeed(
          {DataSources.Entities.TownspeopleJsonPath, Geometry});
  EnhancedStoreValue.dispatch(TownspersonActions::TownspeopleSeeded()(TownspersonSeeds));

  const TArray<FHorseRouteSeed> HorseRouteSeeds =
      BotsAdapters::BuildHorseRouteSeed(
          {DataSources.Entities.HorsesJsonPath, Geometry});
  EnhancedStoreValue.dispatch(HorseActions::HorsesSeeded()(HorseRouteSeeds));

  const TArray<FFeatureSeed> NatureFeatureSeeds =
      NatureAdapters::BuildNatureSeed(
          {DataSources.Entities.NatureJsonPath, Geometry});
  EnhancedStoreValue.dispatch(NatureActions::NatureSeeded()(NatureFeatureSeeds));

  const FRuntimeState &State = EnhancedStoreValue.getState();
  TestTrue(Labels.Next(),
           RuntimeSelectors::SelectTerrainLoaded(State));
  const ecs::FWorld &EcsWorld = RuntimeSelectors::SelectWorld(State);
  TestTrue(Labels.Next(),
           EcsWorld.Generation > int64{});
  TestAuthoredDomainRegistry(this, Labels.Next(), EcsWorld.Domains, Settings);
  TestTrue(Labels.Next(),
           RuntimeSelectors::SelectEntityInDomain(
               State, {StoreAutomation.Terrain.TerrainEntity,
                       DomainKeyFromPath(
                           StoreAutomation.Terrain.TerrainProjectionDomain)}));
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectLandmarks(State).Num(),
            Landmarks.Num());

  const func::Maybe<FLandmark> PostOffice =
      RuntimeSelectors::SelectLandmarkById(State, FirstLandmark.Id);
  TestTrue(Labels.Next(), PostOffice.hasValue);
  check(PostOffice.hasValue);
  TestEqual(Labels.Next(), PostOffice.value.Label,
            FirstLandmark.Label);

  const FPointPayload Spawn =
      RuntimeSelectors::SelectPlayerSpawn(State);
  TestEqual(Labels.Next(), Spawn.AnchorLabel,
            LevelLayoutAdapters::PlayerSpawnAnchorLabel(Geometry));
  TestTrue(Labels.Next(),
           Spawn.Location.Z >
               TerrainData.GetTerrainZAtWorld(
                   LevelLayoutAdapters::PlayerSpawnPoint(Geometry).EastWest,
                   LevelLayoutAdapters::PlayerSpawnPoint(Geometry).NorthSouth));

  TestTrue(Labels.Next(), !HorseRouteSeeds.IsEmpty());
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectHorses(State).Num(),
            HorseRouteSeeds.Num());
  const FHorsePresentationViewModel &HorsePresentation =
      RuntimeSelectors::SelectHorsePresentation(State);
  const FVector &MountedRiderOffsetFeet =
      Settings.HorsePresentation.Rider.MountedRiderOffsetFeet;
  const FVector AuthoredMountedRiderOffset = FVector(
      LevelLayoutAdapters::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.X)}),
      LevelLayoutAdapters::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.Y)}),
      LevelLayoutAdapters::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.Z)}));
  TestTrue(Labels.Next(),
           HorsePresentation.Scale.MountedRiderLocation.Equals(
               AuthoredMountedRiderOffset));
  TestTrue(Labels.Next(),
           HorsePresentation.Scale.MountedRiderLocation.Z <
               HorsePresentation.NameText.Location.Z);
  const FString HorsePatrolLoopLabel = Labels.Next();
  const int32 AuthoredPatrolRoutePointCount =
      RequiredHorseByMountedRider(HorseRouteSeeds, false).PatrolRoute.Num();
  func::for_each_indexed(
      HorseRouteSeeds, static_cast<size_t>(HorseRouteSeeds.Num()),
      [this, &HorsePatrolLoopLabel, AuthoredPatrolRoutePointCount](
          const FHorseRouteSeed &HorseRoute) {
        TestEqual(HorsePatrolLoopLabel, HorseRoute.PatrolRoute.Num(),
                  AuthoredPatrolRoutePointCount);
      });

  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectTownspeople(State).Num(),
            TownspersonSeeds.Num());

  const TArray<FTownspersonSeed> DialogueTownspeople =
      TownspersonReducers::ReduceByInteractionIntent(
          {TownspersonSeeds, ETownspersonInteractionIntent::Dialogue});
  const TArray<FTownspersonSeed> MemoryTownspeople =
      TownspersonReducers::ReduceByInteractionIntent(
          {TownspersonSeeds, ETownspersonInteractionIntent::Memory});
  const TArray<FTownspersonSeed> CombatValidationTownspeople =
      TownspersonReducers::ReduceByInteractionIntent(
          {TownspersonSeeds, ETownspersonInteractionIntent::CombatValidation});
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectDialogueTownspeople(State).Num(),
            DialogueTownspeople.Num());
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectMemoryTownspeople(State).Num(),
            MemoryTownspeople.Num());
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectCombatValidationTownspeople(State).Num(),
            CombatValidationTownspeople.Num());

  const int32 BotEntityCount =
      TownspersonSeeds.Num() + HorseRouteSeeds.Num();
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectBots(State).Num(), BotEntityCount);
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectBotStats(State).Num(), BotEntityCount);
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectBotPositions(State).Num(), BotEntityCount);
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectBotAI(State).Num(), BotEntityCount);
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectBotGoals(State).Num(), BotEntityCount);

  const FTownspersonSeed &DialogueTownsperson =
      RequiredTownspersonWithIntent(TownspersonSeeds,
                                    ETownspersonInteractionIntent::Dialogue);
  const func::Maybe<FBotEntity> Clara =
      RuntimeSelectors::SelectBotById(State, DialogueTownsperson.Id);
  TestTrue(Labels.Next(), Clara.hasValue);
  check(Clara.hasValue);
  TestTrue(Labels.Next(),
           Clara.value.Kind == EBotEntityKind::Townsperson);
  TestTrue(Labels.Next(),
           Clara.value.Alignment == EBotAlignment::Friendly);
  const ecs::FEntityInspection ClaraInspection =
      RuntimeSelectors::SelectEntityInspection(
          State, EntitiesAdapters::BotEntityKey(Clara.value.Id));
  TestTrue(Labels.Next(),
           !ClaraInspection.Components.IsEmpty());
  TestTrue(Labels.Next(),
           RuntimeSelectors::SelectBotById(State, DialogueTownsperson.Id)
               .value.DisplayName == DialogueTownsperson.Name);

  const FHorseRouteSeed &UnmountedHorse =
      RequiredHorseByMountedRider(HorseRouteSeeds, false);
  const func::Maybe<FBotStatsComponent> SorrelStats =
      RuntimeSelectors::SelectBotStatsById(State, UnmountedHorse.Id);
  TestTrue(Labels.Next(), SorrelStats.hasValue);
  check(SorrelStats.hasValue);
  TestTrue(Labels.Next(),
           SorrelStats.value.bMountedRider == UnmountedHorse.bMountedRider);
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(SorrelStats.value.MoveSpeed,
                                Settings.Bot.HorseStats.MoveSpeed));
  const ecs::FEntityInspection SorrelInspection =
      RuntimeSelectors::SelectEntityInspection(
          State, EntitiesAdapters::BotEntityKey(SorrelStats.value.Id));
  TestTrue(Labels.Next(),
           !SorrelInspection.Components.IsEmpty());
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(SorrelStats.value.MoveSpeed,
                                Settings.Bot.HorseStats.MoveSpeed));

  const FHorseRouteSeed &MountedHorse =
      RequiredHorseByMountedRider(HorseRouteSeeds, true);
  const FBotStrategicGoal *PostRoadGoal =
      RuntimeSelectors::SelectBotActiveGoalsById(State).Find(MountedHorse.Id);
  TestTrue(Labels.Next(),
           PostRoadGoal != nullptr);
  check(PostRoadGoal);
  TestTrue(Labels.Next(),
           PostRoadGoal->Type == EBotGoalType::Patrol);
  TestTrue(Labels.Next(),
           PostRoadGoal->bHasTargetLocation);

  TestEqual(Labels.Next(),
            NatureFeatureSeeds.Num(),
            RuntimeSelectors::SelectNatureFeatures(State).Num());
  TestEqual(Labels.Next(),
            RuntimeSelectors::SelectNatureFeatures(State).Num(),
            NatureFeatureSeeds.Num());

  return true;
}
