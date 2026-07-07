#include "Features/Systems/Runtime/RuntimeFactories.h"

#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Entities/Characters/Player/PlayerReducers.h"
#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Systems/Bots/AI/BotAIFactories.h"
#include "Features/Systems/Bots/BotFactories.h"
#include "Features/Systems/Bots/Core/BotCoreSlice.h"
#include "Features/Systems/Bots/Goals/BotGoalFactories.h"
#include "Features/Systems/Bots/Position/BotPositionFactories.h"
#include "Features/Systems/Bots/Stats/BotStatsFactories.h"
#include "Features/Systems/Bots/Horses/HorseFactories.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorSlice.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesSlice.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineSlice.h"
#include "Features/Systems/Dialogue/DialogueReducers.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Level/LevelSlice.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"
#include "Features/Systems/Nature/NatureFactories.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Runtime/RuntimeReducers.h"
#include "Features/Systems/Spawn/SpawnFactories.h"
#include "Features/Systems/Speech/SpeechReducers.h"
#include "Features/Systems/Speech/SpeechSlice.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/Bots/Townspeople/TownspersonFactories.h"
#include "Features/Systems/Bots/Townspeople/TownspersonReducers.h"
#include "Features/Systems/UI/UISharedReducers.h"
#include "Features/Systems/UI/UIChatReducers.h"
#include "Features/Systems/UI/UIConversationReducers.h"
#include "Features/Systems/UI/UISlice.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Interaction/InteractionReducers.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeFactories {

FRuntimeState CreateInitialState() {
  const ForbocAI::Game::Data::FRuntimeSettings Settings =
      ForbocAI::Game::Data::RuntimeSettingsAdapters::LoadRuntimeSettings();
  FRuntimeState State;
  State.Player = PlayerSlice::CreateInitialState();
  State.Systems = SystemsSlice::CreateInitialState();
  State.Level = LevelSystemSlice::CreateInitialState();
  State.Rendering = RenderingSlice::CreateInitialState();
  State.Dialogue = DialogueSlice::CreateInitialState();
  State.Dialogue =
      DialogueReducers::ReduceRuntimeSettings(State.Dialogue,
                                              Settings.DialogueRuntime);
  State.Interaction = InteractionSlice::CreateInitialState();
  State.Speech = SpeechSlice::CreateInitialState();
  State.Speech =
      SpeechReducers::ReduceRuntimeSettings(State.Speech,
                                            Settings.SpeechRuntime);
  State.UI = UISlice::CreateInitialState();
  State.UI.RuntimeSettings = Settings.UIRuntime;
  State.UI.Conversation =
      UIReducers::ReduceRuntimeConversationPlaceholder(Settings.UIRuntime);
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
  State.RuntimeObservationIds = Settings.RuntimeObservationIds;
  State.RuntimeDebugMessages = Settings.RuntimeDebugMessages;
  State.RuntimeViewNames = Settings.RuntimeViewNames;
  State.RuntimeText = Settings.RuntimeText;
  State.BotRuntime = Settings.BotRuntime;
  State.Level.TerrainSources = Settings.LevelTerrainSources;
  State.Level.DataSources = Settings.LevelDataSources;
  State.Level.Geometry = Settings.LevelGeometry;
  State.Player.Presentation =
      PlayerReducers::ReducePlayerPresentation(Settings.PlayerPresentation);
  State.Rendering.RuntimeProfile =
      RenderingProfileReducers::ReduceRuntimeProfile(Settings.RenderingProfile);
  State.Rendering.TextureCatalog =
      RenderingTextureReducers::ReduceTextureCatalog(Settings.TextureCatalog);
  State.Rendering.DistanceLodStages =
      RenderingDistanceLodReducers::ReduceDistanceLodStages(
          Settings.RenderingDistanceLod);
  State.Rendering.RuntimeSettings = Settings.RenderingRuntime;
  State.Rendering.AssetPaths =
      RenderingReducers::ReduceRenderingAssetPaths(Settings.RenderingAssets);
  State.Rendering.TownspersonPresentation =
      RenderingPresentationReducers::ReduceTownspersonPresentation(
          {Settings.TownspersonPresentation, Settings.LevelGeometry});
  State.Rendering.HorsePresentation =
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
  State.Townspeople.LastViewDefaults =
      TownspersonReducers::ReduceViewDefaults(Settings.TownspersonDefaults);
  return RuntimeReducers::ReduceRuntimeProjected(State);
}

} // namespace RuntimeFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
