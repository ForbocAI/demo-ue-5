#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/CharactersPlayerTypes.h"
#include "Features/Systems/Bots/AI/AITypes.h"
#include "Features/Systems/Bots/SystemsBotsTypes.h"
#include "Features/Systems/Bots/Core/BotsCoreTypes.h"
#include "Features/Systems/Bots/Goals/GoalsTypes.h"
#include "Features/Systems/Bots/Position/PositionTypes.h"
#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Orchestrator/OrchestratorTypes.h"
#include "Features/Systems/Bots/Orchestrator/Factories/FactoriesTypes.h"
#include "Features/Systems/Bots/Pipeline/BotsPipelineTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"
#include "Features/Systems/Dialogue/SystemsDialogueTypes.h"
#include "Features/Systems/Interaction/SystemsInteractionTypes.h"
#include "Features/Systems/Level/SystemsLevelTypes.h"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"
#include "Features/Systems/Nature/SystemsNatureTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Lifecycle/LifecycleTypes.h"
#include "Features/Systems/Spawn/SystemsSpawnTypes.h"
#include "Features/Systems/Speech/SystemsSpeechTypes.h"
#include "Features/Systems/Observed/ObservedTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"
#include "Features/Systems/UI/SystemsUITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeEcsState {
  ecs::FWorld World = ecs::createWorld();
};

struct FRuntimeReducerDiagnosticsState {
  FString ReducedActionType;
  double CombinedReducerMilliseconds = double{};
  double ProjectionMilliseconds = double{};
  double RootReducerMilliseconds = double{};
  int32 ProjectedEntityCount = int32{};
  int32 ProjectedComponentTypeCount = int32{};
};

struct FRuntimeState {
  ForbocAI::Game::Data::FObservationIdSettings ObservationIds;
  ForbocAI::Game::Data::FDebugMessageSettings DebugMessages;
  ForbocAI::Game::Data::FViewNameSettings ViewNames;
  ForbocAI::Game::Data::FTextSettings Text;
  ForbocAI::Game::Data::FBotSettings Bot;
  FRuntimeEcsState Ecs;
  FRuntimeReducerDiagnosticsState ReducerDiagnostics;
  FRuntimeLifecycleState Lifecycle;
  FRuntimeTownspersonInteractionRequest TownspersonInteractionRequest;
  FPlayerState Player;
  FSystemsState Systems;
  FLevelSystemState Level;
  FRenderingState Rendering;
  FDialogueState Dialogue;
  FInteractionState Interaction;
  FSpeechState Speech;
  FUIState UI;
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

inline bool operator==(const FRuntimeEcsState &Left,
                       const FRuntimeEcsState &Right) {
  return Left.World == Right.World;
}

inline bool operator!=(const FRuntimeEcsState &Left,
                       const FRuntimeEcsState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeReducerDiagnosticsState &Left,
                       const FRuntimeReducerDiagnosticsState &Right) {
  return Left.ReducedActionType == Right.ReducedActionType &&
         FMath::IsNearlyEqual(Left.CombinedReducerMilliseconds,
                              Right.CombinedReducerMilliseconds) &&
         FMath::IsNearlyEqual(Left.ProjectionMilliseconds,
                              Right.ProjectionMilliseconds) &&
         FMath::IsNearlyEqual(Left.RootReducerMilliseconds,
                              Right.RootReducerMilliseconds) &&
         Left.ProjectedEntityCount == Right.ProjectedEntityCount &&
         Left.ProjectedComponentTypeCount ==
             Right.ProjectedComponentTypeCount;
}

inline bool operator!=(const FRuntimeReducerDiagnosticsState &Left,
                       const FRuntimeReducerDiagnosticsState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeState &Left,
                       const FRuntimeState &Right) {
  return Left.ObservationIds == Right.ObservationIds &&
         Left.DebugMessages == Right.DebugMessages &&
         Left.ViewNames == Right.ViewNames &&
         Left.Text == Right.Text &&
         Left.Bot == Right.Bot &&
         Left.Ecs == Right.Ecs &&
         Left.ReducerDiagnostics == Right.ReducerDiagnostics &&
         Left.Lifecycle == Right.Lifecycle &&
         Left.TownspersonInteractionRequest ==
             Right.TownspersonInteractionRequest &&
         Left.Player == Right.Player &&
         Left.Terrain == Right.Terrain &&
         Left.Systems == Right.Systems && Left.Level == Right.Level &&
         Left.Rendering == Right.Rendering &&
         Left.Dialogue == Right.Dialogue &&
         Left.Interaction == Right.Interaction &&
         Left.Speech == Right.Speech &&
         Left.UI == Right.UI &&
         Left.Spawn == Right.Spawn &&
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
} // namespace Game
} // namespace ForbocAI
