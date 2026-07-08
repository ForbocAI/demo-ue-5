#include "Features/Systems/SystemsSlice.h"

#include "Features/Systems/Bot/BotSlice.h"
#include "Features/Systems/Interaction/Conversation/InteractionConversationSlice.h"

#include "Core/frmt.hpp"
#include "Core/ecs.hpp"
#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Systems/Bots/AI/AIActions.h"
#include "Features/Systems/Bots/SystemsBotsActions.h"
#include "Features/Systems/Bots/Goals/GoalsActions.h"
#include "Features/Systems/Bots/Goals/GoalsSlice.h"
#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/Bots/Stats/StatsActions.h"
#include "Features/Systems/Bots/Stats/StatsSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Interaction/InteractionActions.h"
#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"
#include "Features/Systems/Level/SystemsLevelSlice.h"
#include "Features/Systems/Nature/SystemsNatureActions.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/UI/Shared/SharedSlice.h"
#include "Features/Systems/UI/Chat/ChatSlice.h"
#include "Features/Systems/UI/Conversation/UIConversationSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {
namespace {

typedef TFunction<func::Maybe<FRuntimeState>(
    const FRuntimeState &, const rtk::AnyAction &)>
    FRuntimeActionReducer;

template <typename Payload>
using TRuntimePayloadReducer =
    FRuntimeState (*)(const FRuntimeState &,
                      const rtk::PayloadAction<Payload> &);

struct FRuntimeActionReducerDeclaration {
  FRuntimeActionReducer Reduce;
};

struct FRuntimeActionReducerPlan {};

template <typename Catalog> struct TRuntimeReducerRegistry;

struct FRuntimeActionReducerRequest {
  const FRuntimeState &State;
  const rtk::AnyAction &Action;
};

template <typename Payload>
func::Maybe<FRuntimeState> ReduceExtractedPayloadAction(
    FRuntimeState State,
    const rtk::PayloadAction<Payload> &PayloadAct,
    TRuntimePayloadReducer<Payload> Reduce) {
  return func::just(Reduce(State, PayloadAct));
}

template <typename Payload>
FRuntimeActionReducerDeclaration RuntimePayloadReducer(
    const rtk::ActionCreator<Payload> &Creator,
    TRuntimePayloadReducer<Payload> Reduce) {
  return {[Creator, Reduce](const FRuntimeState &State,
                            const rtk::AnyAction &Action) {
    return func::mbind(
        Creator.extract(Action),
        [&State, &Action, Reduce](const Payload &PayloadValue) {
          return ReduceExtractedPayloadAction(
              State,
              rtk::PayloadAction<Payload>{Action.Type, PayloadValue},
              Reduce);
        });
  }};
}

func::Maybe<FRuntimeState> ReduceRuntimeActionMaybe(
    const FRuntimeActionReducerRequest &Request,
    const TArray<FRuntimeActionReducerDeclaration> &Declarations) {
  return func::fold_array<FRuntimeActionReducerDeclaration,
                          func::Maybe<FRuntimeState>>(
      Declarations, func::nothing<FRuntimeState>(),
      [&Request](const func::Maybe<FRuntimeState> &Current,
                 const FRuntimeActionReducerDeclaration &Declaration) {
        return func::match(
            Current,
            [](const FRuntimeState &Reduced) {
              return func::just(Reduced);
            },
            [&Request, &Declaration]() {
              return Declaration.Reduce(Request.State, Request.Action);
            });
      });
}

} // namespace

#include "Features/Systems/Bot/BotSlice.h"

namespace {

template <> struct TRuntimeReducerRegistry<FRuntimeActionReducerPlan> {
  static const TArray<FRuntimeActionReducerDeclaration> &Declarations() {
    static const TArray<FRuntimeActionReducerDeclaration>
        RegisteredDeclarations = {
            RuntimePayloadReducer(
                RuntimeActions::TownspersonInteractionSourceObserved(),
                ReduceTownspersonInteractionSourceObserved),
            RuntimePayloadReducer(TownspersonActions::TownspeopleSeeded(),
                                  ReduceRuntimeTownspeopleSeeded),
            RuntimePayloadReducer(HorseActions::HorsesSeeded(),
                                  ReduceRuntimeHorsesSeeded)};
    return RegisteredDeclarations;
  }
};

} // namespace

FRuntimeState ReduceRuntimeAction(const FRuntimeState &State,
                                  const rtk::AnyAction &Action) {
  return func::or_else(
      ReduceRuntimeActionMaybe(
          {State, Action},
          TRuntimeReducerRegistry<
              FRuntimeActionReducerPlan>::Declarations()),
      State);
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Systems/Bots/AI/AISlice.h"
#include "Features/Systems/Bots/SystemsBotsSlice.h"
#include "Features/Systems/Bots/Core/CoreSlice.h"
#include "Features/Systems/Bots/Goals/GoalsSlice.h"
#include "Features/Systems/Bots/Horses/HorsesSlice.h"
#include "Features/Systems/Bots/Orchestrator/OrchestratorSlice.h"
#include "Features/Systems/Bots/Orchestrator/Factories/FactoriesSlice.h"
#include "Features/Systems/Bots/Pipeline/PipelineSlice.h"
#include "Features/Systems/Bots/Position/PositionSlice.h"
#include "Features/Systems/Bots/Stats/StatsSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Level/SystemsLevelSlice.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSlice.h"
#include "Features/Systems/Nature/SystemsNatureSlice.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/Lifecycle/LifecycleSlice.h"
#include "Features/Systems/SystemsThunks.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Speech/SpeechSlice.h"
#include "Features/Systems/Observed/ObservedSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/UI/UISlice.h"
#include "HAL/PlatformTime.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSlice {
namespace {

double RuntimeMilliseconds(double ElapsedSeconds,
                           const FRuntimeState &State) {
  return ElapsedSeconds *
         State.UI.Settings.StatsOverlay.SecondsToMilliseconds;
}

FRuntimeState ReduceRootWithDiagnostics(
    FRuntimeState State,
    const rtk::CaseReducer<FRuntimeState> &CombinedReducers,
    const rtk::AnyAction &Action) {
  const double RootStartedSeconds = FPlatformTime::Seconds();
  const double CombinedStartedSeconds = FPlatformTime::Seconds();
  FRuntimeState Combined = CombinedReducers(State, Action);
  const double CombinedFinishedSeconds = FPlatformTime::Seconds();
  FRuntimeState RuntimeReduced =
      RuntimeReducers::ReduceRuntimeAction(Combined, Action);
  const bool bProjectRuntimeWorld =
      RuntimeReducers::ShouldProjectRuntimeAction(Action);
  const double ProjectionStartedSeconds = FPlatformTime::Seconds();
  FRuntimeState Projected =
      bProjectRuntimeWorld
          ? RuntimeReducers::ReduceRuntimeProjected(RuntimeReduced)
          : RuntimeReduced;
  const double ProjectionFinishedSeconds = FPlatformTime::Seconds();
  const double RootFinishedSeconds = FPlatformTime::Seconds();
  const ecs::FWorldInspection Inspection =
      ecs::inspectWorld(Projected.Ecs.World);
  Projected.ReducerDiagnostics = {
      Action.Type,
      RuntimeMilliseconds(CombinedFinishedSeconds - CombinedStartedSeconds,
                          Projected),
      RuntimeMilliseconds(ProjectionFinishedSeconds - ProjectionStartedSeconds,
                          Projected),
      RuntimeMilliseconds(RootFinishedSeconds - RootStartedSeconds, Projected),
      Inspection.Storage.EntityCount,
      Inspection.Storage.ComponentTypeCount};
  return Projected;
}

const rtk::CaseReducer<FRuntimeState> &RootReducer() {
  static const rtk::CaseReducer<FRuntimeState> Reducer = []() {
    rtk::ReducersMapObject<FRuntimeState> Reducers;
    Reducers.reducer(&FRuntimeState::Player, PlayerSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Systems, SystemsSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Level, LevelSystemSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Rendering,
                 RenderingSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Lifecycle,
                 RuntimeLifecycleSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Dialogue,
                 DialogueSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Interaction,
                 InteractionSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Speech, SpeechSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::UI, UISlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Terrain, TerrainSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Spawn, SpawnSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Landmarks, LandmarkSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Townspeople,
                 TownspersonSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Horses, HorseSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Nature, NatureSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::Bots, BotSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotCore, BotCoreSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotStats, BotStatsSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotPosition,
                 BotPositionSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotAI, BotAISlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotGoals, BotGoalSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotOrchestrator,
                 BotOrchestratorSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotOrchestratorFactories,
                 BotOrchestratorFactoriesSlice::GetSlice().Reducer)
        .reducer(&FRuntimeState::BotPipeline,
                 BotPipelineSlice::GetSlice().Reducer);
    const rtk::CaseReducer<FRuntimeState> CombinedReducers =
        rtk::combineReducers(Reducers);
    return [CombinedReducers](const FRuntimeState &State,
                              const rtk::AnyAction &Action) -> FRuntimeState {
      return ReduceRootWithDiagnostics(State, CombinedReducers, Action);
    };
  }();
  return Reducer;
}

} // namespace

const rtk::Slice<FRuntimeState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FRuntimeState>> Slice =
      func::lazy([]() -> rtk::Slice<FRuntimeState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FRuntimeState>(
            TEXT("runtime"), RuntimeFactories::CreateInitialState(),
            RootReducer());
      });
  return func::eval(Slice);
}

rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn() {
  return RuntimeThunks::RequestPlayerSpawn();
}

rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload() {
  return RuntimeThunks::RequestLevelViewPayload();
}

} // namespace RuntimeSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI


#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Systems/Bots/AI/AISlice.h"
#include "Features/Systems/Bots/SystemsBotsSlice.h"
#include "Features/Systems/Bots/Core/CoreSlice.h"
#include "Features/Systems/Bots/Goals/GoalsSlice.h"
#include "Features/Systems/Bots/Position/PositionSlice.h"
#include "Features/Systems/Bots/Stats/StatsSlice.h"
#include "Features/Systems/Bots/Horses/HorsesSlice.h"
#include "Features/Systems/Bots/Orchestrator/OrchestratorSlice.h"
#include "Features/Systems/Bots/Orchestrator/Factories/FactoriesSlice.h"
#include "Features/Systems/Bots/Pipeline/PipelineSlice.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Level/SystemsLevelSlice.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSlice.h"
#include "Features/Systems/Nature/SystemsNatureSlice.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/Lifecycle/LifecycleSlice.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Speech/SpeechSlice.h"
#include "Features/Systems/Speech/SpeechSlice.h"
#include "Features/Systems/Observed/ObservedSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/UI/Shared/SharedSlice.h"
#include "Features/Systems/UI/Chat/ChatSlice.h"
#include "Features/Systems/UI/Conversation/UIConversationSlice.h"
#include "Features/Systems/UI/UISlice.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Interaction/InteractionSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeFactories {

FRuntimeState CreateInitialState() {
  const ForbocAI::Game::Data::FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  FRuntimeState State;
  State.Ecs.World = ecs::createWorld(
      ForbocAI::Game::Data::SettingsAdapters::EcsDomainRegistry(Settings.Ecs));
  State.Player = PlayerSlice::CreateInitialState();
  State.Systems = SystemsSlice::CreateInitialState();
  State.Level = LevelSystemSlice::CreateInitialState();
  State.Rendering = RenderingSlice::CreateInitialState();
  State.Lifecycle = RuntimeLifecycleSlice::CreateInitialState();
  State.Dialogue = DialogueSlice::CreateInitialState();
  State.Dialogue =
      DialogueReducers::ReduceSettings(State.Dialogue, Settings.Dialogue);
  State.Interaction = InteractionSlice::CreateInitialState();
  State.Speech = SpeechSlice::CreateInitialState();
  State.Speech =
      SpeechReducers::ReduceSettings(State.Speech, Settings.Speech);
  State.UI = UISlice::CreateInitialState();
  State.UI.Settings = Settings.UI;
  State.UI.Conversation =
      UIReducers::ReduceRuntimeConversationPlaceholder(Settings.UI);
  State.Terrain = TerrainFactories::CreateInitialState();
  State.Spawn = SpawnFactories::CreateInitialState();
  State.Landmarks = LandmarkFactories::CreateInitialState();
  State.Townspeople = TownspersonFactories::CreateInitialState();
  State.Horses = HorseFactories::CreateInitialState();
  State.Nature = NatureFactories::CreateInitialState();
  State.Bots = BotFactories::CreateInitialState();
  State.BotCore = BotCoreSlice::CreateInitialState();
  State.BotStats = BotStatsFactories::CreateInitialState();
  State.BotPosition = BotPositionFactories::CreateInitialState();
  State.BotAI = BotAIFactories::CreateInitialState();
  State.BotGoals = BotGoalFactories::CreateInitialState();
  State.BotOrchestrator = BotOrchestratorSlice::CreateInitialState();
  State.BotOrchestratorFactories =
      BotOrchestratorFactoriesSlice::CreateInitialState();
  State.BotPipeline = BotPipelineSlice::CreateInitialState();
  State.ObservationIds = Settings.ObservationIds;
  State.DebugMessages = Settings.DebugMessages;
  State.ViewNames = Settings.ViewNames;
  State.Text = Settings.Text;
  State.Bot = Settings.Bot;
  State.Level.TerrainSources = Settings.LevelTerrainSources;
  State.Level.Csv = Settings.LevelCsv;
  State.Level.DataSources = Settings.LevelDataSources;
  State.Level.Geometry = Settings.LevelGeometry;
  State.Player.Presentation =
      PlayerReducers::ReducePlayerPresentation(Settings.PlayerPresentation);
  State.Rendering.Profile.RuntimeProfile =
      RenderingProfileReducers::ReduceRuntimeProfile(Settings.RenderingProfile);
  State.Rendering.Catalog.TextureCatalog =
      RenderingTextureReducers::ReduceTextureCatalog(Settings.TextureCatalog);
  State.Rendering.Catalog.DistanceLodStages =
      RenderingDistanceLodReducers::ReduceDistanceLodStages(
          Settings.RenderingDistanceLod);
  State.Rendering.Profile.Settings = Settings.Rendering;
  State.Rendering.Profile.AssetPaths =
      RenderingReducers::ReduceRenderingAssetPaths(Settings.RenderingAssets);
  State.Rendering.Presentation.TownspersonPresentation =
      RenderingPresentationReducers::ReduceTownspersonPresentation(
          {Settings.TownspersonPresentation, Settings.LevelGeometry});
  State.Rendering.Presentation.HorsePresentation =
      RenderingPresentationReducers::ReduceHorsePresentation(
          {Settings.HorsePresentation, Settings.LevelGeometry});
  State.Interaction.TownspersonMaxDistance =
      InteractionReducers::ReduceTownspersonInteractionDistance(
          {Settings.Interaction, Settings.LevelGeometry});
  State.Interaction.NoTownspersonMessage =
      InteractionReducers::ReduceNoTownspersonMessage(Settings.Interaction);
  State.Interaction.SelectedCandidate =
      InteractionReducers::ReduceEmptySelection(
          State.Interaction.NoTownspersonMessage);
  State.Townspeople.ViewDefaults =
      TownspersonReducers::ReduceViewDefaults(Settings.TownspersonDefaults);
  return RuntimeReducers::ReduceRuntimeProjected(State);
}

} // namespace RuntimeFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
