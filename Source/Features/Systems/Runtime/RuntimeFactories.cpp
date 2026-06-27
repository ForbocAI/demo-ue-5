#include "Features/Systems/Runtime/RuntimeFactories.h"

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
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Level/LevelSlice.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"
#include "Features/Systems/Nature/NatureFactories.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Runtime/RuntimeReducers.h"
#include "Features/Systems/Spawn/SpawnFactories.h"
#include "Features/Systems/Speech/SpeechSlice.h"
#include "Features/Systems/SystemsSlice.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/Bots/Townspeople/TownspersonFactories.h"
#include "Features/Systems/UI/UISlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeFactories {

FRuntimeState CreateInitialState() {
  FRuntimeState State;
  State.Player = PlayerSlice::CreateInitialState();
  State.Systems = SystemsSlice::CreateInitialState();
  State.Level = LevelSystemSlice::CreateInitialState();
  State.Rendering = RenderingSlice::CreateInitialState();
  State.Dialogue = DialogueSlice::CreateInitialState();
  State.Speech = SpeechSlice::CreateInitialState();
  State.UI = UISlice::CreateInitialState();
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
  return RuntimeReducers::ReduceRuntimeEcsProjected(State);
}

} // namespace RuntimeFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
