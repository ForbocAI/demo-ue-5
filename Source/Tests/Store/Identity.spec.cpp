#include "Tests/Store/StoreSpecSupport.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"
#include "Features/Components/Identity/IdentitySlice.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreIdentity,
    FORBOCAI_DEMOUE5_AUTHORED_STRINGV11FF5F0FF033,
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/** User Story: As an ECS identity consumer, I need public store projection and identity adapters verified together so every logical entity retains an inspectable stable identity. @fn bool FStoreIdentity::RunTest(const FString &Parameters) */
bool FStoreIdentity::RunTest(const FString &Parameters) {
  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const FStoreSettings StoreAutomation = Settings.Automation.Store;
  FLabelCursor Labels = StoreLabels(StoreAutomation.Labels.IdentityLabels);
  const ForbocAI::Game::Data::FSourceSettings Sources =
      Settings.LevelTerrainSources;
  const ForbocAI::Game::Data::FCsvSettings Csv = Settings.LevelCsv;
  const ForbocAI::Game::Data::FGeometrySettings Geometry =
      Settings.LevelGeometry;
  FLevelTerrainData TerrainData;
  FLevelOrthoData OrthoData;
  check(TerrainData.LoadFromContent({Sources, Csv, Geometry}));
  check(OrthoData.LoadFromContent({Sources, Csv}));

  rtk::EnhancedStore<ForbocAI::Game::Level::FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  EnhancedStoreValue.dispatch(TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
           TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
           TerrainData.GetMaxElevationMeters()})));

  const ForbocAI::Game::Level::FRuntimeState &State =
      EnhancedStoreValue.getState();
  const ecs::FWorld &EcsWorld = RuntimeSelectors::SelectWorld(State);
  const ecs::EntityKey TerrainEntity =
      StoreAutomation.Terrain.TerrainEntity;
  const TArray<ecs::EntityKey> ProjectedEntities =
      ecs::collectEntityKeys(EcsWorld);
  const TArray<ecs::EntityKey> IdentityEntities =
      ecs::queryEntitiesByDomain(
          {EcsWorld, IdentityAdapters::EntityIdentityDomainKey()});
  TestEqual(Labels.Next(), IdentityEntities.Num(), ProjectedEntities.Num());

  const func::Maybe<FEntityIdentity> TerrainIdentity =
      IdentitySelectors::SelectEntityIdentity(EcsWorld, TerrainEntity);
  TestTrue(Labels.Next(), TerrainIdentity.hasValue);
  check(TerrainIdentity.hasValue);
  TestEqual(Labels.Next(), TerrainIdentity.value.StableId, TerrainEntity);

  const FEntityIdentity ExternalIdentity =
      IdentityAdapters::WithExternalId(
          IdentityAdapters::CreateStableIdentity(TerrainEntity),
          TerrainData.GetSourcePath());
  const ecs::FWorld ExternalIdentityWorld =
      IdentityAdapters::ProjectEntityIdentity({EcsWorld, ExternalIdentity});
  const func::Maybe<FString> SelectedExternalId =
      IdentitySelectors::SelectExternalId(ExternalIdentityWorld,
                                          TerrainEntity);
  check(SelectedExternalId.hasValue);
  TestEqual(Labels.Next(), SelectedExternalId.value,
            TerrainData.GetSourcePath());
  return true;
}
