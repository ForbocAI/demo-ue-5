#include "Features/Systems/Runtime/RuntimeFactories.h"

#include "Features/Systems/Bots/AI/BotAIFactories.h"
#include "Features/Systems/Bots/BotFactories.h"
#include "Features/Systems/Bots/Core/Slice.h"
#include "Features/Systems/Bots/Goals/BotGoalFactories.h"
#include "Features/Systems/Bots/Position/BotPositionFactories.h"
#include "Features/Systems/Bots/Stats/BotStatsFactories.h"
#include "Features/Systems/Bots/Horses/HorseFactories.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorSlice.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesSlice.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineSlice.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"
#include "Features/Systems/Nature/NatureFactories.h"
#include "Features/Systems/Spawn/SpawnFactories.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/Bots/Townspeople/TownspersonFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeFactories {

FRuntimeState CreateInitialState() {
  FRuntimeState State;
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
  return State;
}

} // namespace RuntimeFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
