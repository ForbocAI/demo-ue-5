#include "Features/Systems/Selectors.h"

#include "Features/Entities/Characters/Player/Selectors.h"
#include "Features/Entities/Characters/Player/Slice.h"
#include "Features/Components/Spatial/Level/Layout/Adapters.h"
#include "Features/Systems/Bots/AI/Selectors.h"
#include "Features/Systems/Bots/Selectors.h"
#include "Features/Systems/Bots/Goals/Selectors.h"
#include "Features/Systems/Bots/Position/Slice.h"
#include "Features/Systems/Bots/Position/Selectors.h"
#include "Features/Systems/Bots/Stats/Selectors.h"
#include "Features/Systems/Bots/Horses/Selectors.h"
#include "Features/Systems/Landmarks/Selectors.h"
#include "Features/Systems/Level/Selectors.h"
#include "Features/Systems/Nature/Selectors.h"
#include "Features/Systems/Rendering/Selectors.h"
#include "Features/Systems/Spawn/Selectors.h"
#include "Features/Systems/Terrain/Selectors.h"
#include "Features/Systems/Bots/Townspeople/Slice.h"
#include "Features/Systems/Bots/Townspeople/Selectors.h"
#include "Features/Systems/Interaction/Selectors.h"
#include "Features/Systems/UI/Selectors.h"

#include "Store.h"

namespace ForbocAI {
namespace Game {
namespace Level {

namespace {

using FTownspersonViewDefaultsInputSelector =
    std::function<FTownspersonViewDefaults(const FRuntimeState &)>;

const std::function<FTownspersonViewDefaults(const FRuntimeState &)> &
TownspersonViewDefaultsSelector() {
  static const auto Selector =
      rtk::createSelector<FRuntimeState, FTownspersonViewDefaults>(
          std::make_tuple(FTownspersonViewDefaultsInputSelector(
              [](const FRuntimeState &State) -> FTownspersonViewDefaults {
                return TownspersonSelectors::SelectViewDefaults(
                    State.Townspeople);
              })),
          std::function<FTownspersonViewDefaults(FTownspersonViewDefaults)>(
              [](FTownspersonViewDefaults Defaults)
                  -> FTownspersonViewDefaults {
                return Defaults;
              }));
  return Selector;
}

} // namespace

namespace RuntimeSelectors {

const FRuntimeState &SelectState() { return Store::GetStore().getState(); }

const FPlayerState &SelectPlayerState(const FRuntimeState &State) {
  return State.Player;
}

const FSystemsState &SelectSystemsState(const FRuntimeState &State) {
  return State.Systems;
}

const FLevelSystemState &SelectLevelState(const FRuntimeState &State) {
  return State.Level;
}

const ForbocAI::Game::Data::FLevelTerrainSourceSettings &
SelectLevelTerrainSources(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectTerrainSources(State.Level);
}

const ForbocAI::Game::Data::FLevelDataSourceSettings &
SelectLevelDataSources(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectDataSources(State.Level);
}

const ForbocAI::Game::Data::FLevelGeometrySettings &
SelectLevelGeometry(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectGeometry(State.Level);
}

const FRenderingState &SelectRenderingState(const FRuntimeState &State) {
  return State.Rendering;
}

const FDialogueState &SelectDialogueState(const FRuntimeState &State) {
  return State.Dialogue;
}

const FInteractionState &SelectInteractionState(const FRuntimeState &State) {
  return State.Interaction;
}

const FSpeechState &SelectSpeechState(const FRuntimeState &State) {
  return State.Speech;
}

const FUIState &SelectUIState(const FRuntimeState &State) {
  return State.UI;
}

const FTerrainState &SelectTerrainState(const FRuntimeState &State) {
  return State.Terrain;
}

const FSpawnState &SelectSpawnState(const FRuntimeState &State) {
  return State.Spawn;
}

bool SelectTerrainLoaded(const FRuntimeState &State) {
  return TerrainSelectors::SelectLoaded(State.Terrain);
}

TArray<FLandmark> SelectLandmarks(const FRuntimeState &State) {
  return LandmarkSelectors::SelectAll(State.Landmarks);
}

func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id) {
  return LandmarkSelectors::SelectById(State.Landmarks, Id);
}

FSpawnPointPayload SelectPlayerSpawn(const FRuntimeState &State) {
  return SpawnSelectors::SelectPlayerSpawn(State.Spawn);
}

TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectAll(State.Townspeople);
}

func::Maybe<FTownspersonSeed> SelectTownspersonById(
    const FRuntimeState &State, const FString &Id) {
  return TownspersonSelectors::SelectById(State.Townspeople, Id);
}

const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectDialogueTownspeople(State.Townspeople);
}

const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectMemoryTownspeople(State.Townspeople);
}

const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectCombatValidationTownspeople(
      State.Townspeople);
}

TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State) {
  return HorseSelectors::SelectAll(State.Horses);
}

TArray<FNatureFeatureSeed> SelectNatureFeatures(const FRuntimeState &State) {
  return NatureSelectors::SelectAll(State.Nature);
}

const FRuntimeTownspersonInteractionRequest &
SelectTownspersonInteractionRequest(const FRuntimeState &State) {
  return State.TownspersonInteractionRequest;
}

const FInteractionSelection &SelectInteractionSelection(
    const FRuntimeState &State) {
  return InteractionSelectors::SelectSelectedCandidate(State.Interaction);
}

float SelectTownspersonInteractionDistance(const FRuntimeState &State) {
  return InteractionSelectors::SelectTownspersonInteractionDistance(
      State.Interaction);
}

const FPlayerPresentationViewModel &
SelectPlayerPresentation(const FRuntimeState &State) {
  return PlayerSelectors::SelectPresentation(State.Player);
}

const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRuntimeState &State) {
  return RenderingSelectors::SelectRuntimeProfile(State.Rendering);
}

const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRuntimeState &State) {
  return RenderingSelectors::SelectTextureCatalog(State.Rendering);
}

const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRuntimeState &State) {
  return RenderingSelectors::SelectDistanceLodStages(State.Rendering);
}

const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRuntimeState &State) {
  return RenderingSelectors::SelectRenderingSettings(State.Rendering);
}

const FRenderingAssetPaths &
SelectRenderingAssetPaths(const FRuntimeState &State) {
  return RenderingSelectors::SelectRenderingAssetPaths(State.Rendering);
}

int32 SelectBotInitialPatrolIndex(const TArray<FVector> &PatrolRoute) {
  return BotPositionReducers::ReduceInitialPatrolIndex(PatrolRoute);
}

FBotInitialPatrolLocationPayload SelectBotInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request) {
  return BotPositionReducers::ReduceInitialPatrolLocation(Request);
}

FBotPatrolAdvancePayload
SelectBotPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  return BotPositionReducers::ReducePatrolAdvance(Request);
}

FPlayerMovementInputViewModel
SelectPlayerMovementInput(const FPlayerMovementInputRequest &Request) {
  return PlayerReducers::ReduceMovementInput(Request);
}

FVector SelectPostOfficeWorldCenter(const FRuntimeState &State) {
  const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry =
      RuntimeSelectors::SelectLevelGeometry(State);
  const FLevelLocalPoint Point =
      LevelLayoutAdapters::FromPostOfficeLots({Geometry, float(), float(),
                                            float()});
  return FVector(Point.EastWest, Point.NorthSouth, float());
}

FTownspersonViewDefaults
SelectTownspersonViewDefaults(const FRuntimeState &State) {
  return TownspersonViewDefaultsSelector()(State);
}

FTownspersonViewDefaults SelectTownspersonViewDefaults(
    const FRuntimeState &State,
    const FTownspersonViewDefaultsRequest &Request) {
  return TownspersonReducers::ReduceViewDefaults(
      {Request, SelectTownspersonViewDefaults(State)});
}

FTownspersonInteractionOverlapViewModel SelectTownspersonInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request) {
  return TownspersonReducers::ReduceInteractionOverlap(Request);
}

ForbocAI::Game::UI::FRuntimeConversationViewModel
SelectRuntimeConversation(const FRuntimeState &State) {
  return UISelectors::SelectConversation(State.UI);
}

const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectTownspersonPresentation(State.Rendering);
}

const FHorsePresentationViewModel &
SelectHorsePresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectHorsePresentation(State.Rendering);
}

const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRuntimeState &State) {
  return RenderingSelectors::SelectRuntimeStatsPresentation(State.Rendering);
}

const ForbocAI::Game::Data::FObservationIdSettings &
SelectObservationIds(const FRuntimeState &State) {
  return State.ObservationIds;
}

const ForbocAI::Game::Data::FDebugMessageSettings &
SelectDebugMessages(const FRuntimeState &State) {
  return State.DebugMessages;
}

const ForbocAI::Game::Data::FViewNameSettings &
SelectViewNames(const FRuntimeState &State) {
  return State.ViewNames;
}

const ForbocAI::Game::Data::FTextSettings &
SelectText(const FRuntimeState &State) {
  return State.Text;
}

const ForbocAI::Game::Data::FUISettings &
SelectUISettings(const FRuntimeState &State) {
  return UISelectors::SelectSettings(State.UI);
}

const ForbocAI::Game::Data::FMarketingCaptureSettings &
SelectMarketingCaptureSettings(const FRuntimeState &State) {
  return SelectUISettings(State).MarketingCapture;
}

const ForbocAI::Game::Data::FBotSettings &
SelectBotSettings(const FRuntimeState &State) {
  return State.Bot;
}

const FRuntimeReducerDiagnosticsState &
SelectRuntimeReducerDiagnostics(const FRuntimeState &State) {
  return State.ReducerDiagnostics;
}

const FRuntimeLifecycleState &
SelectRuntimeLifecycle(const FRuntimeState &State) {
  return State.Lifecycle;
}

TArray<FBotEntity> SelectBots(const FRuntimeState &State) {
  return BotSelectors::SelectAll(State.Bots);
}

func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State,
                                      const FString &Id) {
  return BotSelectors::SelectById(State.Bots, Id);
}

TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State) {
  return BotStatsSelectors::SelectAll(State.BotStats);
}

func::Maybe<FBotStatsComponent>
SelectBotStatsById(const FRuntimeState &State, const FString &Id) {
  return BotStatsSelectors::SelectById(State.BotStats, Id);
}

TArray<FBotPositionComponent> SelectBotPositions(
    const FRuntimeState &State) {
  return BotPositionSelectors::SelectAll(State.BotPosition);
}

func::Maybe<FBotPositionComponent>
SelectBotPositionById(const FRuntimeState &State, const FString &Id) {
  return BotPositionSelectors::SelectById(State.BotPosition, Id);
}

TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State) {
  return BotAISelectors::SelectAll(State.BotAI);
}

func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State,
                                             const FString &Id) {
  return BotAISelectors::SelectById(State.BotAI, Id);
}

TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State) {
  return BotGoalSelectors::SelectAll(State.BotGoals);
}

func::Maybe<FBotGoalComponent> SelectBotGoalById(
    const FRuntimeState &State, const FString &Id) {
  return BotGoalSelectors::SelectById(State.BotGoals, Id);
}

const TMap<FString, FBotStrategicGoal> &
SelectBotActiveGoalsById(const FRuntimeState &State) {
  return BotGoalSelectors::SelectActiveGoalsById(State.BotGoals);
}

const ecs::FWorld &SelectWorld(const FRuntimeState &State) {
  return State.Ecs.World;
}

func::Maybe<ecs::FComponentValue>
SelectComponent(const FRuntimeState &State,
                   const FEntityComponentLookup &Lookup) {
  return ecs::getComponent(
      ecs::createGetComponentRequest(SelectWorld(State), Lookup.Entity,
                                     Lookup.Type));
}

bool SelectEntityInDomain(const FRuntimeState &State,
                             const FEntityDomainLookup &Lookup) {
  return ecs::isEntityInDomain(ecs::createEntityInDomainRequest(
      SelectWorld(State), Lookup.Entity, Lookup.Domain));
}

ecs::FEntityInspection
SelectEntityInspection(const FRuntimeState &State,
                          const ecs::EntityKey &Entity) {
  return ecs::inspectEntity(SelectWorld(State), Entity);
}

namespace {

struct FLocalRouteBounds {
  FVector2D Min = FVector2D::ZeroVector;
  FVector2D Max = FVector2D::ZeroVector;
  bool bValid = false;
};

FLocalRouteBounds InitialRouteBounds(const FLevelLocalPoint &Point) {
  return {FVector2D(Point.EastWest, Point.NorthSouth),
          FVector2D(Point.EastWest, Point.NorthSouth), true};
}

FLocalRouteBounds ExtendRouteBounds(const FLocalRouteBounds &Bounds,
                                    const FLevelLocalPoint &Point) {
  return {FVector2D(FMath::Min(Bounds.Min.X, Point.EastWest),
                    FMath::Min(Bounds.Min.Y, Point.NorthSouth)),
          FVector2D(FMath::Max(Bounds.Max.X, Point.EastWest),
                    FMath::Max(Bounds.Max.Y, Point.NorthSouth)),
          true};
}

FLocalRouteBounds ReduceRoutePointBounds(FLocalRouteBounds Bounds,
                                         const FLevelLocalPoint &Point) {
  return Bounds.bValid ? ExtendRouteBounds(Bounds, Point)
                       : InitialRouteBounds(Point);
}

FLocalRouteBounds ReduceTownspersonBounds(FLocalRouteBounds Bounds,
                                          const FTownspersonSeed &Seed) {
  return func::fold_indexed(Seed.PatrolRoute,
                            static_cast<size_t>(Seed.PatrolRoute.Num()), Bounds,
                            ReduceRoutePointBounds);
}

FLocalRouteBounds ReduceHorseBounds(FLocalRouteBounds Bounds,
                                    const FHorseRouteSeed &Seed) {
  return func::fold_indexed(Seed.PatrolRoute,
                            static_cast<size_t>(Seed.PatrolRoute.Num()), Bounds,
                            ReduceRoutePointBounds);
}

FLocalRouteBounds DynamicRouteBounds(const FRuntimeState &State) {
  const TArray<FTownspersonSeed> Townspeople =
      RuntimeSelectors::SelectTownspeople(State);
  const TArray<FHorseRouteSeed> Horses = RuntimeSelectors::SelectHorses(State);
  const FLocalRouteBounds TownspersonBounds = func::fold_indexed(
      Townspeople, static_cast<size_t>(Townspeople.Num()), FLocalRouteBounds{},
      ReduceTownspersonBounds);
  return func::fold_indexed(Horses, static_cast<size_t>(Horses.Num()),
                            TownspersonBounds, ReduceHorseBounds);
}

FVector RouteBoundsCenter(const FLocalRouteBounds &Bounds,
                          const FVector &Fallback) {
  return Bounds.bValid ? FVector((Bounds.Min.X + Bounds.Max.X) * 0.5f,
                                 (Bounds.Min.Y + Bounds.Max.Y) * 0.5f, 0.0f)
                       : Fallback;
}

} // namespace

FVector SelectActorRouteBoundsCenter(const FRuntimeState &State,
                                     const FVector &Fallback) {
  return RouteBoundsCenter(DynamicRouteBounds(State), Fallback);
}

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
