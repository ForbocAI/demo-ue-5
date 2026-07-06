#include "CoreMinimal.h"
#include "Core/ecs.hpp"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Components/Level/LevelTypes.h"
#include "Features/Entities/Characters/Bots/BotsAdapters.h"
#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"
#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"
#include "Features/Systems/Level/LevelAdapters.h"
#include "Features/Systems/Bots/Horses/HorseSlice.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Landmarks/LandmarkSlice.h"
#include "Features/Systems/Nature/NatureSlice.h"
#include "Features/Systems/Rendering/RenderingSelectors.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/Runtime/RuntimeFactories.h"
#include "Features/Systems/Runtime/RuntimeReducers.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSlice.h"
#include "Misc/AutomationTest.h"
#include "Misc/OutputDevice.h"
#include "Store.h"

using namespace ForbocAI::Game::Level;

namespace {

/**
 * @brief Request object for recursively searching captured redux log lines.
 */
struct FReduxLoggerLineSearchRequest {
  const TArray<FString> *Lines = nullptr;
  FString Needle;
  int32 Index = 0;
};

/**
 * @brief Captures the runtime redux logger category during automation tests.
 */
class FRuntimeReduxLoggerCaptureDevice final : public FOutputDevice {
public:
  /**
   * @brief Stores the caller-owned line buffer used by Serialize.
   * @signature explicit FRuntimeReduxLoggerCaptureDevice(TArray<FString> &InLines)
   * @param InLines Mutable output collection for matching redux log lines.
   *
   * User Story: As a test author, I need a focused UE log sink so automation
   * can prove redux-logger writes through the same output path as the console.
   */
  explicit FRuntimeReduxLoggerCaptureDevice(TArray<FString> &InLines)
      : Lines(InLines) {}

  /**
   * @brief Records redux logger messages emitted through UE logging.
   * @signature void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity, const FName &Category)
   * @param V The log message text.
   * @param Verbosity UE log verbosity for the emitted line.
   * @param Category UE log category that produced the line.
   *
   * User Story: As automation coverage, I need to observe UE console output
   * without changing the runtime store or SDK logger behavior.
   */
  void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity,
                 const FName &Category) override {
    (void)Verbosity;
    CaptureLine(V, Category);
  }

  void Serialize(const TCHAR *V, ELogVerbosity::Type Verbosity,
                 const FName &Category, double Time) override {
    (void)Verbosity;
    (void)Time;
    CaptureLine(V, Category);
  }

  void SerializeRecord(const UE::FLogRecord &Record) override {
    FOutputDevice::SerializeRecord(Record);
  }

private:
  void CaptureLine(const TCHAR *V, const FName &Category) {
    Category == FName(TEXT("LogForbocRuntimeRedux"))
        ? (Lines.Add(FString(V)), void())
        : void();
  }

  TArray<FString> &Lines;
};

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
                          {Request.Lines, Request.Needle,
                           Request.Index + 1}));
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
      {Request.Lines, Request.Needle, 0});
}

} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FRuntimeStoreDataBackedMap,
    "ForbocAI.Runtime.Store.DataBackedMap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * @brief Verifies the runtime store projects JSON-backed terrain and feature data into RTK and ECS state.
 * @signature bool FRuntimeStoreDataBackedMap::RunTest(const FString &Parameters)
 * @param Parameters Automation test parameters supplied by Unreal.
 * @return true when the runtime store selectors and ECS projections match seeded data.
 *
 * User Story: As a runtime maintainer, I need the runtime store to prove that
 * reducers own gameplay state and ECS projection while views remain display-only.
 */
bool FRuntimeStoreDataBackedMap::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FRuntimeSettings Settings =
      ForbocAI::Game::Data::RuntimeSettingsAdapters::LoadRuntimeSettings();
  const ForbocAI::Game::Data::FLevelTerrainSourceSettings Sources =
      Settings.LevelTerrainSources;
  const ForbocAI::Game::Data::FLevelDataSourceSettings DataSources =
      Settings.LevelDataSources;
  const ForbocAI::Game::Data::FRuntimeValidationSettings Validation =
      Settings.RuntimeValidation;
  const ForbocAI::Game::Data::FLevelGeometrySettings Geometry =
      Settings.LevelGeometry;
  FLevelTerrainData TerrainData;
  FLevelOrthoData OrthoData;

  TestTrue(TEXT("French Gulch heightmap CSV loads"),
           TerrainData.LoadFromContent({Sources, Geometry}));
  TestTrue(TEXT("French Gulch USGS ortho CSV loads"),
           OrthoData.LoadFromContent({Sources}));
  TestEqual(TEXT("Terrain grid matches authored validation data"),
            TerrainData.GetGridSize(), Validation.TerrainGridSize);
  TestEqual(TEXT("Ortho grid matches authored validation data"),
            OrthoData.GetGridSize(), Validation.OrthoGridSize);
  TestTrue(TEXT("Terrain has relief"),
           TerrainData.GetMaxElevationMeters() >
               TerrainData.GetMinElevationMeters() +
                   Validation.TerrainMinReliefMeters);

  const float ActorWorldUnitsPerFoot =
      LevelLayoutSlice::ActorWorldUnitsFromFeet({Geometry, 1.0f});
  const float BlockWorldUnitsPerFoot =
      Geometry.BlockScalePerFoot * Geometry.CubeMeshSize;
  TestTrue(TEXT("Blockout feet match actor feet"),
           FMath::IsNearlyEqual(BlockWorldUnitsPerFoot,
                                ActorWorldUnitsPerFoot));
  const float StoryWorldUnits =
      Geometry.HeightScalePerStory * Geometry.CubeMeshSize;
  const float CharacterHeightFeet =
      Settings.TownspersonPresentation.CharacterHeightFeet;
  TestTrue(TEXT("Blockout story clears a character"),
           StoryWorldUnits >
               LevelLayoutSlice::ActorWorldUnitsFromFeet(
                   {Geometry, CharacterHeightFeet}));

  const FLevelRuntimeLayoutSeed RuntimeLayout =
      LevelAdapters::LoadRuntimeLayoutSeed(DataSources.RuntimeLayoutJsonPath);
  TestEqual(TEXT("Runtime layout loads town labels"),
            RuntimeLayout.Town.Labels.Num(), 1);
  check(RuntimeLayout.Town.Labels.Num() > 0);
  TestEqual(TEXT("Runtime layout parses label height_mode"),
            static_cast<int32>(RuntimeLayout.Town.Labels[0].Height),
            static_cast<int32>(ELevelRuntimeLabelHeightMode::Explicit));
  TestEqual(TEXT("Runtime layout loads overlay labels"),
            RuntimeLayout.OverlayLabels.Num(), 1);
  TestTrue(TEXT("Runtime layout loads OSM road and creek blocks"),
           RuntimeLayout.Terrain.Blocks.Num() >= 20);
  TestTrue(TEXT("Runtime layout loads OSM building footprints"),
           RuntimeLayout.Town.Blocks.Num() >= 20);
  TestTrue(TEXT("Runtime layout includes Trinity Mountain Road"),
           func::any_indexed(
               RuntimeLayout.Terrain.Blocks,
               static_cast<size_t>(RuntimeLayout.Terrain.Blocks.Num()),
               [](const FLevelRuntimeBlockSeed &Block) {
                 return Block.Name.Contains(TEXT("Trinity Mountain Road"));
               }));
  TestTrue(TEXT("Runtime layout includes Clear Creek"),
           func::any_indexed(
               RuntimeLayout.Terrain.Blocks,
               static_cast<size_t>(RuntimeLayout.Terrain.Blocks.Num()),
               [](const FLevelRuntimeBlockSeed &Block) {
                 return Block.Name.Contains(TEXT("Clear Creek"));
               }));

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
  EnhancedStoreValue.dispatch(LandmarkActions::LandmarksSeeded()(Landmarks));
  EnhancedStoreValue.dispatch(SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {LevelLayoutSlice::ToWorld(
               {TerrainData, LevelLayoutSlice::PlayerSpawnPoint(Geometry)}),
           LevelLayoutSlice::PlayerSpawnRotation(Geometry),
           LevelLayoutSlice::PlayerSpawnAnchorLabel(Geometry)})));

  const TArray<FTownspersonSeed> TownspersonSeeds =
      BotsAdapters::BuildTownspersonSeed(
          {DataSources.TownspeopleJsonPath, Geometry});
  EnhancedStoreValue.dispatch(TownspersonActions::TownspeopleSeeded()(TownspersonSeeds));

  const TArray<FHorseRouteSeed> HorseRouteSeeds =
      BotsAdapters::BuildHorseRouteSeed({DataSources.HorsesJsonPath, Geometry});
  EnhancedStoreValue.dispatch(HorseActions::HorsesSeeded()(HorseRouteSeeds));

  const TArray<FNatureFeatureSeed> NatureFeatureSeeds =
      NatureAdapters::BuildNatureSeed({DataSources.NatureJsonPath, Geometry});
  EnhancedStoreValue.dispatch(NatureActions::NatureSeeded()(NatureFeatureSeeds));

  const FRuntimeState &State = EnhancedStoreValue.getState();
  TestTrue(TEXT("RTK slice records terrain load"),
           RuntimeSelectors::SelectTerrainLoaded(State));
  const ecs::FWorld &EcsWorld = RuntimeSelectors::SelectWorld(State);
  TestTrue(TEXT("Runtime store owns a projected ECS world"),
           EcsWorld.Generation > 0);
  TestTrue(TEXT("ECS registry includes Systems/Bots domain"),
           EcsWorld.Domains.Nodes.Contains(ecs::createDomainPathKey(
               ecs::createDomainPath({TEXT("Systems"), TEXT("Bots")}))));
  TestTrue(TEXT("ECS registry includes Systems/Projection/Bots/Stats domain"),
           EcsWorld.Domains.Nodes.Contains(ecs::createDomainPathKey(
               ecs::createDomainPath({TEXT("Systems"), TEXT("Projection"),
                                      TEXT("Bots"), TEXT("Stats")}))));
  TestTrue(TEXT("ECS registry includes Entities/Characters/Bots domain"),
           EcsWorld.Domains.Nodes.Contains(ecs::createDomainPathKey(
               ecs::createDomainPath({TEXT("Entities"), TEXT("Characters"),
                                      TEXT("Bots")}))));
  TestTrue(TEXT("Terrain entity is projected through Systems/Projection/Terrain"),
           RuntimeSelectors::SelectEntityInDomain(
               State, TEXT("level:terrain"),
               ecs::createDomainPathKey(ecs::createDomainPath(
                   {TEXT("Systems"), TEXT("Projection"), TEXT("Terrain")}))));
  TestEqual(TEXT("RTK entity adapter stores seeded landmarks"),
            RuntimeSelectors::SelectLandmarks(State).Num(),
            Landmarks.Num());

  const func::Maybe<FLandmark> PostOffice =
      RuntimeSelectors::SelectLandmarkById(State, TEXT("post-office"));
  TestTrue(TEXT("Post office landmark is selectable"), PostOffice.hasValue);
  check(PostOffice.hasValue);
  TestEqual(TEXT("Post office label"), PostOffice.value.Label,
            FString(TEXT("U.S. Post Office / 14200 Trinity Mountain Rd")));

  const FSpawnPointPayload Spawn =
      RuntimeSelectors::SelectPlayerSpawn(State);
  TestEqual(TEXT("Player spawn anchor"), Spawn.AnchorLabel,
            LevelLayoutSlice::PlayerSpawnAnchorLabel(Geometry));
  TestTrue(TEXT("Player spawn is above terrain"),
           Spawn.Location.Z >
               TerrainData.GetTerrainZAtWorld(
                   LevelLayoutSlice::PlayerSpawnPoint(Geometry).EastWest,
                   LevelLayoutSlice::PlayerSpawnPoint(Geometry).NorthSouth));

  TestEqual(TEXT("1899 horse routes are seeded"), HorseRouteSeeds.Num(), 3);
  TestEqual(TEXT("RTK entity adapter stores seeded horses"),
            RuntimeSelectors::SelectHorses(State).Num(),
            HorseRouteSeeds.Num());
  const FHorsePresentationViewModel &HorsePresentation =
      RuntimeSelectors::SelectHorsePresentation(State);
  const FVector &MountedRiderOffsetFeet =
      Settings.HorsePresentation.MountedRiderOffsetFeet;
  const FVector AuthoredMountedRiderOffset = FVector(
      LevelLayoutSlice::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.X)}),
      LevelLayoutSlice::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.Y)}),
      LevelLayoutSlice::ActorWorldUnitsFromFeet(
          {Geometry, static_cast<float>(MountedRiderOffsetFeet.Z)}));
  TestTrue(TEXT("Mounted rider offset is authored in JSON"),
           HorsePresentation.MountedRiderLocation.Equals(
               AuthoredMountedRiderOffset));
  TestTrue(TEXT("Mounted rider sits below horse label"),
           HorsePresentation.MountedRiderLocation.Z <
               HorsePresentation.NameTextLocation.Z);
  func::for_each_indexed(
      HorseRouteSeeds, static_cast<size_t>(HorseRouteSeeds.Num()),
      [this](const FHorseRouteSeed &HorseRoute) {
        TestTrue(FString::Printf(TEXT("%s has a patrol loop"), *HorseRoute.Name),
                 HorseRoute.PatrolRoute.Num() >= 3);
      });

  TestEqual(TEXT("RTK entity adapter stores seeded townspeople"),
            RuntimeSelectors::SelectTownspeople(State).Num(),
            TownspersonSeeds.Num());

  TestEqual(TEXT("Runtime has dialogue townsperson interaction"),
            RuntimeSelectors::SelectDialogueTownspeople(State).Num(),
            1);
  TestEqual(TEXT("Runtime has memory townsperson interaction"),
            RuntimeSelectors::SelectMemoryTownspeople(State).Num(),
            1);
  TestEqual(TEXT("Runtime has combat validation townsperson interaction"),
            RuntimeSelectors::SelectCombatValidationTownspeople(State).Num(),
            1);

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
  check(Clara.hasValue);
  TestTrue(TEXT("Clara is a townsperson bot"),
           Clara.value.Kind == EBotEntityKind::Townsperson);
  TestTrue(TEXT("Clara is friendly"),
           Clara.value.Alignment == EBotAlignment::Friendly);
  TestTrue(TEXT("Clara is projected into the ECS townspeople domain"),
           RuntimeSelectors::SelectEntityInDomain(
               State, TEXT("bot:clara-bell"),
               ecs::createDomainPathKey(ecs::createDomainPath(
                   {TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
                    TEXT("Townspeople")}))));
  const func::Maybe<ecs::FComponentValue> ClaraRole =
      RuntimeSelectors::SelectComponent(
          State, TEXT("bot:clara-bell"), TEXT("Components/Bots/Role"));
  TestTrue(TEXT("Clara role is projected as an ECS component"),
           ClaraRole.hasValue);
  check(ClaraRole.hasValue);
  TestEqual(TEXT("Clara ECS role"), ClaraRole.value.TextValue,
            FString(TEXT("Postmaster")));

  const func::Maybe<FBotStatsComponent> SorrelStats =
      RuntimeSelectors::SelectBotStatsById(State, TEXT("sorrel-at-livery"));
  TestTrue(TEXT("Horse bot stats are selectable"), SorrelStats.hasValue);
  check(SorrelStats.hasValue);
  TestTrue(TEXT("Sorrel is not a mounted rider"),
           !SorrelStats.value.bMountedRider);
  TestTrue(TEXT("Sorrel has route movement speed"),
           SorrelStats.value.MoveSpeed > 0.0f);
  TestTrue(TEXT("Sorrel is projected into the ECS horse domain"),
           RuntimeSelectors::SelectEntityInDomain(
               State, TEXT("bot:sorrel-at-livery"),
               ecs::createDomainPathKey(ecs::createDomainPath(
                   {TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
                    TEXT("Horses")}))));
  const func::Maybe<ecs::FComponentValue> SorrelMoveSpeed =
      RuntimeSelectors::SelectComponent(
          State, TEXT("bot:sorrel-at-livery"),
          TEXT("Components/Stats/MoveSpeed"));
  TestTrue(TEXT("Sorrel move speed is projected as an ECS component"),
           SorrelMoveSpeed.hasValue);
  check(SorrelMoveSpeed.hasValue);
  TestTrue(TEXT("Sorrel ECS move speed is positive"),
           SorrelMoveSpeed.value.FloatValue > 0.0f);

  const FBotStrategicGoal *PostRoadGoal =
      RuntimeSelectors::SelectBotActiveGoalsById(State).Find(
          TEXT("post-road-rider"));
  TestTrue(TEXT("Horse bot active goal is selectable"),
           PostRoadGoal != nullptr);
  check(PostRoadGoal);
  TestTrue(TEXT("Post road rider has a patrol goal"),
           PostRoadGoal->Type == EBotGoalType::Patrol);
  TestTrue(TEXT("Post road rider goal targets a route location"),
           PostRoadGoal->bHasTargetLocation);

  TestEqual(TEXT("OSM-backed layout removes fictional nature markers"),
            NatureFeatureSeeds.Num(), 0);
  TestEqual(TEXT("RTK entity adapter stores seeded nature features"),
            RuntimeSelectors::SelectNatureFeatures(State).Num(),
            NatureFeatureSeeds.Num());

  const FLevelRetroRenderProfile &RetroProfile =
      RenderingSelectors::SelectRuntimeProfile(
          RuntimeSelectors::SelectRenderingState(State));
  TestEqual(TEXT("Retro profile is set to 11pm"),
            RetroProfile.TimeOfDayHour, 23.0f);
  TestEqual(TEXT("Retro profile applies authored full-moon saturation"),
            RetroProfile.PostProcessSaturationMultiplier, 0.72f);
  TestEqual(TEXT("Retro profile applies authored full-moon contrast"),
            RetroProfile.PostProcessContrastMultiplier, 1.14f);
  TestTrue(TEXT("Retro profile uses desaturated full-moon light"),
           RetroProfile.DirectionalLightColorR >=
               RetroProfile.DirectionalLightColorB);
  TestEqual(TEXT("Retro profile disables anti-aliasing"),
            RetroProfile.AntiAliasingMethod, 0);
  TestEqual(TEXT("Retro profile renders at native low resolution"),
            RetroProfile.ScreenPercentage, 100.0f);
  TestEqual(TEXT("Retro profile targets a 320px internal render width"),
            RetroProfile.InternalRenderWidth, 320);
  TestEqual(TEXT("Retro profile targets a 240px internal render height"),
            RetroProfile.InternalRenderHeight, 240);
  TestEqual(TEXT("Retro profile scales the internal render target up from JSON"),
            RetroProfile.OutputScaleMultiplier, 4);
  TestTrue(TEXT("Retro profile scales the internal render target fullscreen"),
           RetroProfile.bFullscreenOutput);
  TestTrue(TEXT("Retro profile keeps view distance bounded for performance"),
           RetroProfile.ViewDistanceScale <= 0.65f);
  TestTrue(TEXT("Retro profile uses a built-in night sky dome"),
           RetroProfile.bSkyDomeEnabled &&
               RetroProfile.SkyDomeMeshPath.Contains(TEXT("/Engine/")) &&
               RetroProfile.SkyDomeMaterialPath.Contains(TEXT("Night")));
  TestTrue(TEXT("Retro profile uses a dark-blue reference night sky"),
           RetroProfile.SkyDomeHorizonColorB >
                   RetroProfile.SkyDomeZenithColorB &&
               RetroProfile.SkyDomeSkyBrightness >= 0.4f);
  TestTrue(TEXT("Retro profile drives bright point stars from JSON"),
           RetroProfile.SkyDomeStarColorR > 10.0f &&
               RetroProfile.SkyDomeStarColorB >
                   RetroProfile.SkyDomeHorizonColorB);
  TestTrue(TEXT("Retro profile uses procedural moon pixel geometry"),
           RetroProfile.bMoonDiscEnabled &&
               RetroProfile.MoonDiscMeshPath.Contains(TEXT("Cube")) &&
               RetroProfile.MoonDiscMaterialPath.Contains(TEXT("Unlit")));
  TestTrue(TEXT("Retro profile keeps reference haze light and blue"),
           RetroProfile.bFogEnabled && RetroProfile.FogDensity <= 0.006f &&
               RetroProfile.FogMaxOpacity <= 0.28f &&
               RetroProfile.FogColorB > RetroProfile.FogColorR);
  TestFalse(TEXT("Retro profile keeps volumetric fog disabled"),
            RetroProfile.bVolumetricFogEnabled);

  const TArray<FLevelDistanceLodStage> &DistanceLodStages =
      RenderingSelectors::SelectDistanceLodStages(
          RuntimeSelectors::SelectRenderingState(State));
  TestEqual(TEXT("Runtime has five distance LOD stages"),
            DistanceLodStages.Num(), 5);
  TestTrue(TEXT("Nearest LOD keeps actors visible and active"),
           DistanceLodStages[0].bStaticVisible &&
               DistanceLodStages[0].bDynamicVisible &&
               DistanceLodStages[0].bPatrolEnabled &&
               DistanceLodStages[0].bAnimated);
  TestTrue(TEXT("Far LOD stops labels, animation, and patrol"),
           !DistanceLodStages[2].bLabelsVisible &&
               !DistanceLodStages[2].bAnimated &&
               !DistanceLodStages[2].bPatrolEnabled);
  TestFalse(TEXT("Terminal LOD stage culls distant actors"),
            DistanceLodStages[4].bStaticVisible ||
                DistanceLodStages[4].bDynamicVisible);
  TestTrue(TEXT("Silhouette LOD keeps dynamic models visible as low-cost shapes"),
           DistanceLodStages[3].bDynamicVisible &&
               !DistanceLodStages[3].bAnimated &&
               !DistanceLodStages[3].bPatrolEnabled);

  const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &StatsOverlay =
      RuntimeSelectors::SelectUIRuntimeSettings(State).StatsOverlay;
  TestEqual(TEXT("Stats overlay reads authored memory divisor"),
            StatsOverlay.MemoryBytesPerMegabyte, 1048576);
  TestEqual(TEXT("Stats overlay shows physical memory"),
            StatsOverlay.UsedPhysicalMemoryLabel, FString(TEXT("MEM:")));
  TestEqual(TEXT("Stats overlay shows peak physical memory"),
            StatsOverlay.PeakPhysicalMemoryLabel, FString(TEXT("PEAK:")));

  const TArray<FLevelRetroTextureSpec> &TextureCatalog =
      RenderingSelectors::SelectTextureCatalog(
          RuntimeSelectors::SelectRenderingState(State));
  TestTrue(TEXT("Retro texture catalog covers terrain, level, NPC, and horse surfaces"),
           TextureCatalog.Num() >= 13);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FRuntimeStoreReduxLoggerMiddleware,
    "ForbocAI.Runtime.Store.ReduxLoggerMiddleware",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * @brief Verifies the SDK redux-logger middleware writes RTK dispatch logs through UE output.
 * @signature bool FRuntimeStoreReduxLoggerMiddleware::RunTest(const FString &Parameters)
 * @param Parameters Automation test parameters supplied by Unreal.
 * @return true when a dispatched action is captured from the UE log output path.
 *
 * User Story: As a developer debugging tests or runtime play, I need RTK
 * actions to be visible in automation logs and the UE console from one store.
 */
bool FRuntimeStoreReduxLoggerMiddleware::RunTest(const FString &Parameters) {
  (void)Parameters;

  TArray<FString> CapturedLines;
  FRuntimeReduxLoggerCaptureDevice CaptureDevice(CapturedLines);

  GLog->AddOutputDevice(&CaptureDevice);
  rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  EnhancedStoreValue.dispatch(RuntimeActions::RuntimeHydrated()(
      RuntimeFactories::CreateInitialState()));
  GLog->FlushThreadedLogs();
  GLog->RemoveOutputDevice(&CaptureDevice);

  TestTrue(TEXT("Redux logger writes action title to UE automation output"),
           ContainsReduxLoggerLine(
               {&CapturedLines, TEXT("action runtime/hydrated"), 0}));
  TestTrue(TEXT("Redux logger writes action payload row to UE automation output"),
           ContainsReduxLoggerLine({&CapturedLines, TEXT("action    "), 0}));

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FRuntimeStoreProjectionGate,
    "ForbocAI.Runtime.Store.ProjectionGate",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * @brief Verifies high-frequency patrol observation actions do not rebuild ECS.
 * @signature bool FRuntimeStoreProjectionGate::RunTest(const FString &Parameters)
 * @param Parameters Automation test parameters supplied by Unreal.
 * @return true when projected ECS input actions are gated separately from view-only patrol calculations.
 *
 * User Story: As a runtime maintainer, I need RTK reducers to update hot
 * observation state without rebuilding ECS unless the dispatched action mutates
 * a slice selected by the ECS projection.
 */
bool FRuntimeStoreProjectionGate::RunTest(const FString &Parameters) {
  (void)Parameters;

  FBotPositionMoved MovePayload;
  FBotPatrolAdvanceRequest PatrolRequest;
  TestTrue(TEXT("Bot movement actions are ECS projection inputs"),
           RuntimeReducers::ShouldProjectRuntimeAction(
               BotPositionActions::BotPositionMoved()(MovePayload)));
  TestFalse(TEXT("Patrol advance observations are not ECS projection inputs"),
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
  TestEqual(TEXT("Patrol advance observation keeps current ECS projection"),
            ObservedGeneration, ProjectedGeneration);

  return true;
}
