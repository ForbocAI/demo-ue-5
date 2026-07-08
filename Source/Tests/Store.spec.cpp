#include "CoreMinimal.h"
#include "Core/ecs.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Entities/Characters/Bots/CharactersBotsAdapters.h"
#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"
#include "Features/Entities/Environments/Landmarks/EnvironmentsLandmarksAdapters.h"
#include "Features/Systems/Level/SystemsLevelAdapters.h"
#include "Features/Systems/Bots/Horses/HorsesSlice.h"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSlice.h"
#include "Features/Systems/Nature/SystemsNatureSlice.h"
#include "Features/Systems/Rendering/Distance/Lod/LodSlice.h"
#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Misc/AutomationTest.h"
#include "Misc/OutputDevice.h"
#include "Store.h"

using namespace ForbocAI::Game::Level;

namespace {

using ForbocAI::Game::Data::FSettings;
using FStoreSettings =
    ForbocAI::Game::Data::Automation::Store::FSettings;

/**
 * @brief Request object for recursively searching captured redux log lines.
 */
struct FReduxLoggerLineSearchRequest {
  const TArray<FString> *Lines = nullptr;
  FString Needle;
  int32 Index = int32{};
};

/**
 * @brief Captures the runtime redux logger category during automation tests.
 */
class FReduxLoggerCaptureDevice final : public FOutputDevice {
public:
  /**
   * @brief Stores the caller-owned line buffer used by Serialize.
   * @signature explicit FReduxLoggerCaptureDevice(TArray<FString> &InLines)
   * @param InLines Mutable output collection for matching redux log lines.
   *
   * User Story: As a test author, I need a focused UE log sink so automation
   * can prove redux-logger writes through the same output path as the console.
   */
  explicit FReduxLoggerCaptureDevice(TArray<FString> &InLines,
                                     const FString &InCategory)
      : Lines(InLines), Category(*InCategory) {}

  /**
   * @brief Records redux logger messages emitted through UE logging.
   * @signature void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity, const FName &LineCategory)
   * @param V The log message text.
   * @param Verbosity UE log verbosity for the emitted line.
   * @param LineCategory UE log category that produced the line.
   *
   * User Story: As automation coverage, I need to observe UE console output
   * without changing the runtime store or SDK logger behavior.
   */
  void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity,
                 const FName &LineCategory) override {
    (void)Verbosity;
    CaptureLine(V, LineCategory);
  }

  void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity,
                 const FName &LineCategory, double Time) override {
    (void)Verbosity;
    (void)Time;
    CaptureLine(V, LineCategory);
  }

  void SerializeRecord(const UE::FLogRecord &Record) override {
    FOutputDevice::SerializeRecord(Record);
  }

private:
  void CaptureLine(const TCHAR *V, const FName &LineCategory) {
    LineCategory == Category
        ? (Lines.Add(FString(V)), void())
        : void();
  }

  TArray<FString> &Lines;
  FName Category;
};

struct FAutomationTestLabelCursor {
  const TArray<FString> *Labels = nullptr;
  int32 Index = int32();

  FString Next() {
    check(Labels != nullptr);
    check(Labels->IsValidIndex(Index));
    const FString Label = (*Labels)[Index];
    ++Index;
    return Label;
  }
};

FAutomationTestLabelCursor
AutomationTestLabels(const TArray<FString> &Labels) {
  return {&Labels, int32()};
}

const FLandmark &RequiredFirstLandmark(const TArray<FLandmark> &Landmarks) {
  check(!Landmarks.IsEmpty());
  return Landmarks[Landmarks.Num() - Landmarks.Num()];
}

const FTownspersonSeed &RequiredTownspersonWithIntent(
    const TArray<FTownspersonSeed> &Townspeople,
    ETownspersonInteractionIntent Intent) {
  const FTownspersonSeed *Townsperson =
      Townspeople.FindByPredicate(
          [Intent](const FTownspersonSeed &Candidate) {
            return Candidate.InteractionIntent == Intent;
          });
  check(Townsperson);
  return *Townsperson;
}

const FHorseRouteSeed &RequiredHorseByMountedRider(
    const TArray<FHorseRouteSeed> &Horses, bool bMountedRider) {
  const FHorseRouteSeed *Horse =
      Horses.FindByPredicate([bMountedRider](const FHorseRouteSeed &Candidate) {
        return Candidate.bMountedRider == bMountedRider;
      });
  check(Horse);
  return *Horse;
}

TArray<FString> DomainSegments(const FString &Path) {
  TArray<FString> Segments;
  const FString Separator = FString::Chr(TCHAR('/'));
  Path.ParseIntoArray(Segments, *Separator, true);
  return Segments;
}

ecs::DomainPathKey DomainKeyFromPath(const FString &Path) {
  return ecs::createDomainPathKey(ecs::createDomainPath(DomainSegments(Path)));
}

FString ReduxLoggerActionTitleNeedle(const FString &Prefix,
                                     const rtk::AnyAction &Action) {
  return Prefix + FString::Chr(TCHAR(' ')) + Action.Type;
}

void TestAuthoredDomainRegistry(FAutomationTestBase *Test,
                                const FString &Label,
                                const ecs::FDomainGraph &Registry,
                                const FSettings &Settings) {
  func::for_each_indexed(
      Settings.Ecs.DomainRegistry,
      static_cast<size_t>(Settings.Ecs.DomainRegistry.Num()),
      [Test, &Label, &Registry](
          const ForbocAI::Game::Data::FEcsDomainRegistrationSettings &Domain) {
        Test->TestTrue(Label,
                       Registry.Nodes.Contains(DomainKeyFromPath(Domain.Path)));
      });
}

FReduxLoggerLineSearchRequest
NextReduxLoggerLineSearchRequest(const FReduxLoggerLineSearchRequest &Request) {
  FReduxLoggerLineSearchRequest Next = Request;
  ++Next.Index;
  return Next;
}

/**
 * @brief Recursively checks captured redux logger lines for a substring.
 * @signature bool ContainsReduxLoggerLineRecursive(const FReduxLoggerLineSearchRequest &Request)
 * @param Request Search payload containing lines, substring, and current index.
 * @return true when the substring appears in any captured line.
 *
 * User Story: As a test maintainer, I need a small pure helper so logger
 * assertions stay separate from the UE output-device effect boundary.
 */
bool ContainsReduxLoggerLineRecursive(
    const FReduxLoggerLineSearchRequest &Request) {
  return Request.Index >= Request.Lines->Num()
             ? false
	             : ((*Request.Lines)[Request.Index].Contains(Request.Needle)
	                    ? true
	                    : ContainsReduxLoggerLineRecursive(
		                          NextReduxLoggerLineSearchRequest(Request)));
}

/**
 * @brief Checks captured redux logger lines for a substring.
 * @signature bool ContainsReduxLoggerLine(const FReduxLoggerLineSearchRequest &Request)
 * @param Request Search payload containing lines and substring.
 * @return true when the substring appears in any captured line.
 *
 * User Story: As automation coverage, I need readable assertions that prove
 * dispatched RTK actions reach the redux-logger UE output sink.
 */
bool ContainsReduxLoggerLine(const FReduxLoggerLineSearchRequest &Request) {
  return ContainsReduxLoggerLineRecursive(
      {Request.Lines, Request.Needle, int32{}});
}

} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreDataBackedMap,
    "ForbocAI.Runtime.Store.DataBackedMap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * @brief Verifies the runtime store projects JSON-backed terrain and feature data into RTK and ECS state.
 * @signature bool FStoreDataBackedMap::RunTest(const FString &Parameters)
 * @param Parameters Automation test parameters supplied by Unreal.
 * @return true when the runtime store selectors and ECS projections match seeded data.
 *
 * User Story: As a runtime maintainer, I need the runtime store to prove that
 * reducers own gameplay state and ECS projection while views remain display-only.
 */
bool FStoreDataBackedMap::RunTest(const FString &Parameters) {
  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const FStoreSettings StoreAutomation = Settings.Automation.Store;
  FAutomationTestLabelCursor Labels = AutomationTestLabels(
      StoreAutomation.DataBackedMapLabels);
  const ForbocAI::Game::Data::FTerrainSourceSettings Sources =
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
      Settings.TownspersonPresentation.CharacterHeightFeet;
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
          {DataSources.LandmarksJsonPath, TerrainData, Geometry});
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
          {DataSources.TownspeopleJsonPath, Geometry});
  EnhancedStoreValue.dispatch(TownspersonActions::TownspeopleSeeded()(TownspersonSeeds));

  const TArray<FHorseRouteSeed> HorseRouteSeeds =
      BotsAdapters::BuildHorseRouteSeed({DataSources.HorsesJsonPath, Geometry});
  EnhancedStoreValue.dispatch(HorseActions::HorsesSeeded()(HorseRouteSeeds));

  const TArray<FFeatureSeed> NatureFeatureSeeds =
      NatureAdapters::BuildNatureSeed({DataSources.NatureJsonPath, Geometry});
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
               State, {StoreAutomation.TerrainEntity,
                       DomainKeyFromPath(StoreAutomation.TerrainProjectionDomain)}));
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
      Settings.HorsePresentation.MountedRiderOffsetFeet;
  const FVector AuthoredMountedRiderOffset = FVector(
      LevelLayoutAdapters::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.X)}),
      LevelLayoutAdapters::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.Y)}),
      LevelLayoutAdapters::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.Z)}));
  TestTrue(Labels.Next(),
           HorsePresentation.MountedRiderLocation.Equals(
               AuthoredMountedRiderOffset));
  TestTrue(Labels.Next(),
           HorsePresentation.MountedRiderLocation.Z <
               HorsePresentation.NameTextLocation.Z);
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

  const FLevelRetroRenderProfile &RetroProfile =
      RenderingSelectors::SelectRuntimeProfile(
          RuntimeSelectors::SelectRenderingState(State));
  const ForbocAI::Game::Data::FProfileSettings &ExpectedProfile =
      Settings.RenderingProfile;
  TestEqual(Labels.Next(),
            RetroProfile.TimeOfDayHour, ExpectedProfile.TimeOfDayHour);
  TestEqual(Labels.Next(),
            RetroProfile.PostProcessSaturationMultiplier,
            ExpectedProfile.PostProcessSaturationMultiplier);
  TestEqual(Labels.Next(),
            RetroProfile.PostProcessContrastMultiplier,
            ExpectedProfile.PostProcessContrastMultiplier);
  TestTrue(Labels.Next(),
           RetroProfile.DirectionalLightColorR >=
               RetroProfile.DirectionalLightColorB);
  TestEqual(Labels.Next(),
            RetroProfile.AntiAliasingMethod,
            ExpectedProfile.AntiAliasingMethod);
  TestEqual(Labels.Next(),
            RetroProfile.ScreenPercentage, ExpectedProfile.ScreenPercentage);
  TestEqual(Labels.Next(),
            RetroProfile.InternalRenderWidth,
            ExpectedProfile.InternalRenderWidth);
  TestEqual(Labels.Next(),
            RetroProfile.InternalRenderHeight,
            ExpectedProfile.InternalRenderHeight);
  TestEqual(Labels.Next(),
            RetroProfile.OutputScaleMultiplier,
            ExpectedProfile.OutputScaleMultiplier);
  TestTrue(Labels.Next(),
           RetroProfile.bFullscreenOutput == ExpectedProfile.bFullscreenOutput);
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(RetroProfile.ViewDistanceScale,
                                ExpectedProfile.ViewDistanceScale));
  TestTrue(Labels.Next(),
           RetroProfile.bSkyDomeEnabled == ExpectedProfile.bSkyDomeEnabled &&
               RetroProfile.SkyDomeMeshPath == ExpectedProfile.SkyDomeMeshPath &&
               RetroProfile.SkyDomeMaterialPath ==
                   ExpectedProfile.SkyDomeMaterialPath);
  TestTrue(Labels.Next(),
           RetroProfile.SkyDomeHorizonColorB >
                   RetroProfile.SkyDomeZenithColorB &&
               FMath::IsNearlyEqual(RetroProfile.SkyDomeSkyBrightness,
                                    ExpectedProfile.SkyDomeSkyBrightness));
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(RetroProfile.SkyDomeStarColorR,
                                ExpectedProfile.SkyDomeStarColorR) &&
               FMath::IsNearlyEqual(RetroProfile.SkyDomeStarColorB,
                                    ExpectedProfile.SkyDomeStarColorB));
  TestTrue(Labels.Next(),
           RetroProfile.bMoonDiscEnabled == ExpectedProfile.bMoonDiscEnabled &&
               RetroProfile.MoonDiscMeshPath ==
                   ExpectedProfile.MoonDiscMeshPath &&
               RetroProfile.MoonDiscMaterialPath ==
                   ExpectedProfile.MoonDiscMaterialPath);
  TestTrue(Labels.Next(),
           RetroProfile.bFogEnabled == ExpectedProfile.bFogEnabled &&
               FMath::IsNearlyEqual(RetroProfile.FogDensity,
                                    ExpectedProfile.FogDensity) &&
               FMath::IsNearlyEqual(RetroProfile.FogMaxOpacity,
                                    ExpectedProfile.FogMaxOpacity) &&
               FMath::IsNearlyEqual(RetroProfile.FogColorB,
                                    ExpectedProfile.FogColorB));
  TestFalse(Labels.Next(),
            RetroProfile.bVolumetricFogEnabled);

  const TArray<FLevelDistanceLodStage> &DistanceLodStages =
      RenderingSelectors::SelectDistanceLodStages(
          RuntimeSelectors::SelectRenderingState(State));
  const TArray<FLevelDistanceLodStage> ExpectedDistanceLodStages =
      RenderingDistanceLodReducers::ReduceDistanceLodStages(
          Settings.RenderingDistanceLod);
  TestEqual(Labels.Next(),
            DistanceLodStages.Num(), ExpectedDistanceLodStages.Num());
  const FString RuntimeLodStageLabel = Labels.Next();
  func::for_each_indexed(
      ExpectedDistanceLodStages,
      static_cast<size_t>(ExpectedDistanceLodStages.Num()),
      [this, &DistanceLodStages,
       &RuntimeLodStageLabel](const FLevelDistanceLodStage &ExpectedStage) {
        const FLevelDistanceLodStage *RuntimeStage =
            DistanceLodStages.FindByPredicate(
                [&ExpectedStage](const FLevelDistanceLodStage &Candidate) {
                  return Candidate.Id == ExpectedStage.Id;
                });
        TestTrue(RuntimeLodStageLabel,
                 RuntimeStage != nullptr && *RuntimeStage == ExpectedStage);
      });

  const ForbocAI::Game::Data::FStatsOverlaySettings &StatsOverlay =
      RuntimeSelectors::SelectUISettings(State).StatsOverlay;
  const ForbocAI::Game::Data::FStatsOverlaySettings
      &ExpectedStatsOverlay = Settings.UI.StatsOverlay;
  TestEqual(Labels.Next(),
            StatsOverlay.MemoryBytesPerMegabyte,
            ExpectedStatsOverlay.MemoryBytesPerMegabyte);
  TestEqual(Labels.Next(),
            StatsOverlay.UsedPhysicalMemoryLabel,
            ExpectedStatsOverlay.UsedPhysicalMemoryLabel);
  TestEqual(Labels.Next(),
            StatsOverlay.PeakPhysicalMemoryLabel,
            ExpectedStatsOverlay.PeakPhysicalMemoryLabel);

  const TArray<FLevelRetroTextureSpec> &TextureCatalog =
      RenderingSelectors::SelectTextureCatalog(
          RuntimeSelectors::SelectRenderingState(State));
  TestTrue(Labels.Next(),
           TextureCatalog.Num() >= Settings.TextureCatalog.Num());

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreReduxLoggerMiddleware,
    "ForbocAI.Runtime.Store.ReduxLoggerMiddleware",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * @brief Verifies the SDK redux-logger middleware writes RTK dispatch logs through UE output.
 * @signature bool FStoreReduxLoggerMiddleware::RunTest(const FString &Parameters)
 * @param Parameters Automation test parameters supplied by Unreal.
 * @return true when a dispatched action is captured from the UE log output path.
 *
 * User Story: As a developer debugging tests or runtime play, I need RTK
 * actions to be visible in automation logs and the UE console from one store.
 */
bool FStoreReduxLoggerMiddleware::RunTest(const FString &Parameters) {
  (void)Parameters;

  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const FStoreSettings StoreAutomation = Settings.Automation.Store;
  FAutomationTestLabelCursor Labels = AutomationTestLabels(
      StoreAutomation.ReduxLoggerMiddlewareLabels);
  TArray<FString> CapturedLines;
  FReduxLoggerCaptureDevice CaptureDevice(CapturedLines,
                                          StoreAutomation.ReduxLoggerCategory);

  GLog->AddOutputDevice(&CaptureDevice);
  rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  const rtk::AnyAction HydratedAction = RuntimeActions::RuntimeHydrated()(
      RuntimeFactories::CreateInitialState());
  EnhancedStoreValue.dispatch(HydratedAction);
  GLog->FlushThreadedLogs();
  GLog->RemoveOutputDevice(&CaptureDevice);

  TestTrue(Labels.Next(),
           ContainsReduxLoggerLine(
               {&CapturedLines,
                ReduxLoggerActionTitleNeedle(
                    StoreAutomation.ReduxLoggerActionTitlePrefix,
                    HydratedAction),
                int32{}}));
  TestTrue(Labels.Next(),
           ContainsReduxLoggerLine({&CapturedLines, HydratedAction.Type,
                                    int32{}}));

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreProjectionGate,
    "ForbocAI.Runtime.Store.ProjectionGate",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * @brief Verifies high-frequency patrol observation actions do not rebuild ECS.
 * @signature bool FStoreProjectionGate::RunTest(const FString &Parameters)
 * @param Parameters Automation test parameters supplied by Unreal.
 * @return true when projected ECS input actions are gated separately from view-only patrol calculations.
 *
 * User Story: As a runtime maintainer, I need RTK reducers to update hot
 * observation state without rebuilding ECS unless the dispatched action mutates
 * a slice selected by the ECS projection.
 */
bool FStoreProjectionGate::RunTest(const FString &Parameters) {
  (void)Parameters;

  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  FAutomationTestLabelCursor Labels = AutomationTestLabels(
      Settings.Automation.Store.ProjectionGateLabels);
  FBotPositionMoved MovePayload;
  FBotPatrolAdvanceRequest PatrolRequest;
  TestTrue(Labels.Next(),
           RuntimeReducers::ShouldProjectRuntimeAction(
               BotPositionActions::BotPositionMoved()(MovePayload)));
  TestFalse(Labels.Next(),
            RuntimeReducers::ShouldProjectRuntimeAction(
                BotPositionActions::PatrolAdvanceObserved()(PatrolRequest)));

  rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  const int64 ProjectedGeneration =
      RuntimeSelectors::SelectWorld(EnhancedStoreValue.getState()).Generation;
  EnhancedStoreValue.dispatch(
      BotPositionActions::PatrolAdvanceObserved()(PatrolRequest));
  const int64 ObservedGeneration =
      RuntimeSelectors::SelectWorld(EnhancedStoreValue.getState()).Generation;
  TestEqual(Labels.Next(),
            ObservedGeneration, ProjectedGeneration);

  return true;
}
