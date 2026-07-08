#include "Features/Systems/Initialization/InitializationAdapters.h"

#include "Core/ecs.hpp"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Systems/Bots/AI/AISlice.h"
#include "Features/Systems/Bots/Behavior/BehaviorSlice.h"
#include "Features/Systems/Bots/Goals/GoalsSlice.h"
#include "Features/Systems/Bots/Horses/HorsesSlice.h"
#include "Features/Systems/Bots/Orchestrator/OrchestratorSlice.h"
#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessSlice.h"
#include "Features/Systems/Bots/Pipeline/PipelineSlice.h"
#include "Features/Systems/Bots/Position/PositionSlice.h"
#include "Features/Systems/Bots/Stats/StatsSlice.h"
#include "Features/Systems/Bots/SystemsBotsSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSlice.h"
#include "Features/Systems/Level/SystemsLevelSlice.h"
#include "Features/Systems/Lifecycle/LifecycleSlice.h"
#include "Features/Systems/Nature/SystemsNatureSlice.h"
#include "Features/Systems/Observed/ObservedSlice.h"
#include "Features/Systems/Projection/ProjectionSlice.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/Spawn/SystemsSpawnSlice.h"
#include "Features/Systems/Speech/SpeechSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/UI/Chat/ChatSlice.h"
#include "Features/Systems/UI/Chat/Rendering/ChatRenderingSlice.h"
#include "Features/Systems/UI/Conversation/UIConversationSlice.h"
#include "Features/Systems/UI/UISlice.h"

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
  State.BotOrchestratorReadiness =
      BotOrchestratorReadinessSlice::CreateInitialState();
  State.BotPipeline = BotPipelineSlice::CreateInitialState();
  State.ObservationIds = Settings.ObservationIds;
  State.DebugMessages = Settings.DebugMessages;
  State.ViewNames = Settings.ViewNames;
  State.Text = Settings.Text;
  State.Bot = Settings.Bot;
  State.TownspersonDefaults = Settings.TownspersonDefaults;
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
  return RuntimeReducers::ReduceRuntimeProjected(State);
}

} // namespace RuntimeFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
