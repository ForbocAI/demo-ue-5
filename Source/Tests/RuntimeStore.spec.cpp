#include "CoreMinimal.h"
#include "Core/ecs.hpp"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Components/Level/LevelTypes.h"
#include "Features/Entities/Characters/Bots/BotsAdapters.h"
#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"
#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"
#include "Features/Systems/Bots/Horses/HorseSlice.h"
#include "Features/Systems/Landmarks/LandmarkSlice.h"
#include "Features/Systems/Nature/NatureSlice.h"
#include "Features/Systems/Rendering/RenderingSelectors.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/Runtime/RuntimeFactories.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSlice.h"
#include "Misc/AutomationTest.h"
#include "Misc/OutputDevice.h"
#include "Store.h"

using namespace ForbocAI::Demo::Level;

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
 * @brief Captures the demo redux logger category during automation tests.
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
    Category == FName(TEXT("LogForbocDemoRedux"))
        ? (Lines.Add(FString(V)), void())
        : void();
  }

private:
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
 * User Story: As a demo maintainer, I need the runtime store to prove that
 * reducers own gameplay state and ECS projection while views remain display-only.
 */
bool FRuntimeStoreDataBackedMap::RunTest(const FString &Parameters) {
  const ForbocAI::Demo::Data::FDemoRuntimeSettings Settings =
      ForbocAI::Demo::Data::RuntimeSettingsAdapters::LoadDemoRuntimeSettings();
  const ForbocAI::Demo::Data::FLevelTerrainSourceSettings Sources =
      Settings.LevelTerrainSources;
  const ForbocAI::Demo::Data::FLevelGeometrySettings Geometry =
      Settings.LevelGeometry;
  FLevelTerrainData TerrainData;
  FLevelOrthoData OrthoData;

  TestTrue(TEXT("French Gulch heightmap CSV loads"),
           TerrainData.LoadFromContent({Sources, Geometry}));
  TestTrue(TEXT("French Gulch USGS ortho CSV loads"),
           OrthoData.LoadFromContent({Sources}));
  TestEqual(TEXT("Terrain grid is 65x65"), TerrainData.GetGridSize(), 65);
  TestEqual(TEXT("Ortho grid is 65x65"), OrthoData.GetGridSize(), 65);
  TestTrue(TEXT("Terrain has relief"),
           TerrainData.GetMaxElevationMeters() >
               TerrainData.GetMinElevationMeters() + 250.0f);

  rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  EnhancedStoreValue.dispatch(TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
           TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
           TerrainData.GetMaxElevationMeters()})));

  const TArray<FLandmark> Landmarks =
      LandmarksAdapters::Build1899LandmarkSeed({TerrainData, Geometry});
  EnhancedStoreValue.dispatch(LandmarkActions::LandmarksSeeded()(Landmarks));
  EnhancedStoreValue.dispatch(SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {LevelLayoutSlice::ToWorld(
               {TerrainData, LevelLayoutSlice::PlayerSpawnPoint(Geometry)}),
           LevelLayoutSlice::PlayerSpawnRotation(Geometry),
           LevelLayoutSlice::PlayerSpawnAnchorLabel(Geometry)})));

  const TArray<FTownspersonSeed> TownspersonSeeds =
      BotsAdapters::Build1899TownspersonSeed(Geometry);
  EnhancedStoreValue.dispatch(TownspersonActions::TownspeopleSeeded()(TownspersonSeeds));

  const TArray<FHorseRouteSeed> HorseRouteSeeds =
      BotsAdapters::Build1899HorseRouteSeed(Geometry);
  EnhancedStoreValue.dispatch(HorseActions::HorsesSeeded()(HorseRouteSeeds));

  const TArray<FNatureFeatureSeed> NatureFeatureSeeds =
      NatureAdapters::BuildClearCreekNatureSeed(Geometry);
  EnhancedStoreValue.dispatch(NatureActions::NatureSeeded()(NatureFeatureSeeds));

  const FRuntimeState &State = EnhancedStoreValue.getState();
  TestTrue(TEXT("RTK slice records terrain load"),
           RuntimeSelectors::SelectTerrainLoaded(State));
  const ecs::FWorld &EcsWorld = RuntimeSelectors::SelectEcsWorld(State);
  TestTrue(TEXT("Runtime store owns a projected ECS world"),
           EcsWorld.Generation > 0);
  TestTrue(TEXT("ECS registry includes Systems/Bots domain"),
           EcsWorld.Domains.Nodes.Contains(ecs::domainPathKey(
               ecs::createDomainPath({TEXT("Systems"), TEXT("Bots")}))));
  TestTrue(TEXT("ECS registry includes Systems/Projection/Bots/Stats domain"),
           EcsWorld.Domains.Nodes.Contains(ecs::domainPathKey(
               ecs::createDomainPath({TEXT("Systems"), TEXT("Projection"),
                                      TEXT("Bots"), TEXT("Stats")}))));
  TestTrue(TEXT("ECS registry includes Entities/Characters/Bots domain"),
           EcsWorld.Domains.Nodes.Contains(ecs::domainPathKey(
               ecs::createDomainPath({TEXT("Entities"), TEXT("Characters"),
                                      TEXT("Bots")}))));
  TestTrue(TEXT("Terrain entity is projected through Systems/Projection/Terrain"),
           RuntimeSelectors::SelectEcsEntityInDomain(
               State, TEXT("level:terrain"),
               ecs::domainPathKey(ecs::createDomainPath(
                   {TEXT("Systems"), TEXT("Projection"), TEXT("Terrain")}))));
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
  for (const FHorseRouteSeed &HorseRoute : HorseRouteSeeds) {
    TestTrue(FString::Printf(TEXT("%s has a patrol loop"), *HorseRoute.Name),
             HorseRoute.PatrolRoute.Num() >= 3);
  }

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
  if (Clara.hasValue) {
    TestTrue(TEXT("Clara is a townsperson bot"),
             Clara.value.Kind == EBotEntityKind::Townsperson);
    TestTrue(TEXT("Clara is friendly"),
             Clara.value.Alignment == EBotAlignment::Friendly);
  }
  TestTrue(TEXT("Clara is projected into the ECS townspeople domain"),
           RuntimeSelectors::SelectEcsEntityInDomain(
               State, TEXT("bot:clara-bell"),
               ecs::domainPathKey(ecs::createDomainPath(
                   {TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
                    TEXT("Townspeople")}))));
  const func::Maybe<ecs::FComponentValue> ClaraRole =
      RuntimeSelectors::SelectEcsComponent(
          State, TEXT("bot:clara-bell"), TEXT("Components/Bots/Role"));
  TestTrue(TEXT("Clara role is projected as an ECS component"),
           ClaraRole.hasValue);
  if (ClaraRole.hasValue) {
    TestEqual(TEXT("Clara ECS role"), ClaraRole.value.TextValue,
              FString(TEXT("Postmaster")));
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
  TestTrue(TEXT("Sorrel is projected into the ECS horse domain"),
           RuntimeSelectors::SelectEcsEntityInDomain(
               State, TEXT("bot:sorrel-at-livery"),
               ecs::domainPathKey(ecs::createDomainPath(
                   {TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
                    TEXT("Horses")}))));
  const func::Maybe<ecs::FComponentValue> SorrelMoveSpeed =
      RuntimeSelectors::SelectEcsComponent(
          State, TEXT("bot:sorrel-at-livery"),
          TEXT("Components/Stats/MoveSpeed"));
  TestTrue(TEXT("Sorrel move speed is projected as an ECS component"),
           SorrelMoveSpeed.hasValue);
  if (SorrelMoveSpeed.hasValue) {
    TestTrue(TEXT("Sorrel ECS move speed is positive"),
             SorrelMoveSpeed.value.FloatValue > 0.0f);
  }

  const FBotStrategicGoal *PostRoadGoal =
      RuntimeSelectors::SelectBotActiveGoalsById(State).Find(
          TEXT("post-road-rider"));
  TestTrue(TEXT("Horse bot active goal is selectable"),
           PostRoadGoal != nullptr);
  if (PostRoadGoal) {
    TestTrue(TEXT("Post road rider has a patrol goal"),
             PostRoadGoal->Type == EBotGoalType::Patrol);
    TestTrue(TEXT("Post road rider goal targets a route location"),
             PostRoadGoal->bHasTargetLocation);
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

  const FLevelRetroRenderProfile &RetroProfile =
      RenderingSelectors::SelectRuntimeProfile(
          RuntimeSelectors::SelectRenderingState(State));
  TestEqual(TEXT("Retro profile is set to 3pm"),
            RetroProfile.TimeOfDayHour, 15.0f);
  TestEqual(TEXT("Retro profile disables anti-aliasing"),
            RetroProfile.AntiAliasingMethod, 0);
  TestTrue(TEXT("Retro profile keeps long readable vistas"),
           RetroProfile.ViewDistanceScale >= 1.5f);

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
  GLog->RemoveOutputDevice(&CaptureDevice);

  TestTrue(TEXT("Redux logger writes action title to UE automation output"),
           ContainsReduxLoggerLine(
               {&CapturedLines, TEXT("action runtime/hydrated"), 0}));
  TestTrue(TEXT("Redux logger writes action payload row to UE automation output"),
           ContainsReduxLoggerLine({&CapturedLines, TEXT("action    "), 0}));

  return true;
}
