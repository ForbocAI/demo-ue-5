#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/AI/BotAITypes.h"
#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Bots/Core/Types.h"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorTypes.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesTypes.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineTypes.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"
#include "Features/Systems/Nature/NatureTypes.h"
#include "Features/Systems/Spawn/SpawnTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FRuntimeState {
  FTerrainState Terrain;
  FSpawnState Spawn;
  FLandmarkState Landmarks;
  FTownspersonState Townspeople;
  FHorseState Horses;
  FNatureState Nature;
  FBotState Bots;
  FBotCoreState BotCore;
  FBotStatsState BotStats;
  FBotPositionState BotPosition;
  FBotAIState BotAI;
  FBotGoalState BotGoals;
  FBotOrchestratorState BotOrchestrator;
  FBotOrchestratorFactoriesState BotOrchestratorFactories;
  FBotPipelineState BotPipeline;
};

inline bool operator==(const FRuntimeState &Left,
                       const FRuntimeState &Right) {
  return Left.Terrain == Right.Terrain && Left.Spawn == Right.Spawn &&
         Left.Landmarks == Right.Landmarks &&
         Left.Townspeople == Right.Townspeople &&
         Left.Horses == Right.Horses && Left.Nature == Right.Nature &&
         Left.Bots == Right.Bots && Left.BotCore == Right.BotCore &&
         Left.BotStats == Right.BotStats &&
         Left.BotPosition == Right.BotPosition && Left.BotAI == Right.BotAI &&
         Left.BotGoals == Right.BotGoals &&
         Left.BotOrchestrator == Right.BotOrchestrator &&
         Left.BotOrchestratorFactories == Right.BotOrchestratorFactories &&
         Left.BotPipeline == Right.BotPipeline;
}

inline bool operator!=(const FRuntimeState &Left,
                       const FRuntimeState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
