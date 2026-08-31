#include "Tests/Store/StoreSpecSupport.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"
#include "Features/Components/Identity/IdentitySlice.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreEntityAdapters,
    FORBOCAI_DEMOUE5_AUTHORED_STRINGV191EAE612B59,
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/** User Story: As an entity-adapter consumer, I need public action-to-selector flows verified against the projected root world so entity ownership remains observable and deterministic. @fn bool FStoreEntityAdapters::RunTest(const FString &Parameters) */
bool FStoreEntityAdapters::RunTest(const FString &Parameters) {
  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const FStoreSettings StoreAutomation = Settings.Automation.Store;
  FLabelCursor Labels =
      StoreLabels(StoreAutomation.Labels.EntityAdapterLabels);
  const ForbocAI::Game::Data::FDataSourceSettings DataSources =
      Settings.LevelDataSources;
  const ForbocAI::Game::Data::FGeometrySettings Geometry =
      Settings.LevelGeometry;
  FLevelTerrainData TerrainData;
  check(TerrainData.LoadFromContent(
      {Settings.LevelTerrainSources, Settings.LevelCsv, Geometry}));

  const TArray<FLandmark> Landmarks = LandmarksAdapters::BuildLandmarkSeed(
      {DataSources.Entities.LandmarksJsonPath, TerrainData, Geometry});
  const TArray<FFeatureSeed> Nature = NatureAdapters::BuildNatureSeed(
      {DataSources.Entities.NatureJsonPath, Geometry});
  const TArray<FTownspersonSeed> Townspeople =
      BotsAdapters::BuildTownspersonSeed(
          {DataSources.Entities.TownspeopleJsonPath, Geometry});
  const TArray<FHorseRouteSeed> Horses = BotsAdapters::BuildHorseRouteSeed(
      {DataSources.Entities.HorsesJsonPath, Geometry});
  check(!Landmarks.IsEmpty());
  check(!Townspeople.IsEmpty());
  check(!Horses.IsEmpty());

  rtk::EnhancedStore<FRuntimeState> StoreValue = Store::ConfigureStore();
  StoreValue.dispatch(LandmarkActions::LandmarksSeeded()(Landmarks));
  StoreValue.dispatch(NatureActions::NatureSeeded()(Nature));
  StoreValue.dispatch(TownspersonActions::TownspeopleSeeded()(Townspeople));
  StoreValue.dispatch(HorseActions::HorsesSeeded()(Horses));

  const FRuntimeState &State = StoreValue.getState();
  TestEqual(Labels.Next(), RuntimeSelectors::SelectLandmarks(State).Num(),
            Landmarks.Num());
  TestEqual(Labels.Next(), RuntimeSelectors::SelectNatureFeatures(State).Num(),
            Nature.Num());
  TestEqual(Labels.Next(), RuntimeSelectors::SelectTownspeople(State).Num(),
            Townspeople.Num());
  TestEqual(Labels.Next(), RuntimeSelectors::SelectHorses(State).Num(),
            Horses.Num());

  const TArray<FFeatureSeed> NatureProjectionFixture =
      NatureAdapters::BuildNatureSeed(
          {StoreAutomation.EntityAdapters.NatureProjectionFixtureJsonPath,
           Geometry});
  const bool bHasNatureProjectionFixture =
      TestFalse(Labels.Next(), NatureProjectionFixture.IsEmpty());
  return func::match(
      func::from_nullable_value<const TArray<FFeatureSeed> *>(
          &NatureProjectionFixture, bHasNatureProjectionFixture),
      [this, &StoreValue, &Labels, &StoreAutomation, &Landmarks, &Townspeople,
       &Horses](const TArray<FFeatureSeed> *Fixture) {
        StoreValue.dispatch(NatureActions::NatureSeeded()(*Fixture));
        const FRuntimeState &ProjectedState = StoreValue.getState();
        TestEqual(Labels.Next(),
                  RuntimeSelectors::SelectNatureFeatures(ProjectedState).Num(),
                  Fixture->Num());

        const ecs::EntityKey Landmark =
            LandmarksAdapters::LandmarkEntityKey(Landmarks.Last().Id);
        const ecs::EntityKey NatureFeature =
            NatureAdapters::NatureEntityKey(Fixture->Last().Id);
        const ecs::EntityKey Townsperson =
            BotsAdapters::BotEntityKey(Townspeople.Last().Id);
        const ecs::EntityKey Horse =
            BotsAdapters::BotEntityKey(Horses.Last().Id);
        TestTrue(
            Labels.Next(),
            RuntimeSelectors::SelectEntityInDomain(
                ProjectedState,
                {Landmark, DomainKeyFromPath(StoreAutomation.EntityAdapters
                                                  .LandmarkEntityDomain)}));
        TestTrue(
            Labels.Next(),
            RuntimeSelectors::SelectEntityInDomain(
                ProjectedState,
                {NatureFeature, DomainKeyFromPath(StoreAutomation.EntityAdapters
                                                       .NatureEntityDomain)}));
        TestTrue(
            Labels.Next(),
            RuntimeSelectors::SelectEntityInDomain(
                ProjectedState,
                {Townsperson,
                 DomainKeyFromPath(StoreAutomation.EntityAdapters
                                        .TownspersonEntityDomain)}));
        TestTrue(
            Labels.Next(),
            RuntimeSelectors::SelectEntityInDomain(
                ProjectedState,
                {Horse, DomainKeyFromPath(
                            StoreAutomation.EntityAdapters.HorseEntityDomain)}));

        const ecs::FWorld &World =
            RuntimeSelectors::SelectWorld(ProjectedState);
        const TArray<ecs::EntityKey> EntityKeys =
            {Landmark, NatureFeature, Townsperson, Horse};
        TestTrue(Labels.Next(), func::all_array(
                                    EntityKeys,
                                    [&World](const ecs::EntityKey &Entity) {
                                      const func::Maybe<ecs::EntityKey>
                                          StableId =
                                              IdentitySelectors::SelectStableId(
                                                  World, Entity);
                                      return StableId.hasValue &&
                                             StableId.value == Entity;
                                    }));
        return true;
      },
      []() { return false; });
}
