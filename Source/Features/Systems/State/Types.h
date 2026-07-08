#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/Types.h"
#include "Features/Systems/Bots/AI/Types.h"
#include "Features/Systems/Bots/Types.h"
#include "Features/Systems/Bots/Core/Types.h"
#include "Features/Systems/Bots/Goals/Types.h"
#include "Features/Systems/Bots/Position/Types.h"
#include "Features/Systems/Bots/Stats/Types.h"
#include "Features/Systems/Bots/Horses/Types.h"
#include "Features/Systems/Bots/Orchestrator/Types.h"
#include "Features/Systems/Bots/Orchestrator/Factories/Types.h"
#include "Features/Systems/Bots/Pipeline/Types.h"
#include "Features/Systems/Bots/Townspeople/Types.h"
#include "Features/Systems/Dialogue/Types.h"
#include "Features/Systems/Interaction/Types.h"
#include "Features/Systems/Level/Types.h"
#include "Features/Systems/Landmarks/Types.h"
#include "Features/Systems/Nature/Types.h"
#include "Features/Systems/Rendering/Types.h"
#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Lifecycle/Types.h"
#include "Features/Systems/Spawn/Types.h"
#include "Features/Systems/Speech/Types.h"
#include "Features/Systems/Observed/Types.h"
#include "Features/Systems/Terrain/Types.h"
#include "Features/Systems/UI/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeEcsState {
  ecs::FWorld World = ecs::createWorld();
};

struct FRuntimeReducerDiagnosticsState {
  FString LastActionType;
  double LastCombinedReducerMilliseconds = double{};
  double LastProjectionMilliseconds = double{};
  double LastRootReducerMilliseconds = double{};
  int32 LastProjectedEntityCount = int32{};
  int32 LastProjectedComponentTypeCount = int32{};
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
  FRuntimeTownspersonInteractionRequest LastTownspersonInteractionRequest;
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
  return Left.LastActionType == Right.LastActionType &&
         FMath::IsNearlyEqual(Left.LastCombinedReducerMilliseconds,
                              Right.LastCombinedReducerMilliseconds) &&
         FMath::IsNearlyEqual(Left.LastProjectionMilliseconds,
                              Right.LastProjectionMilliseconds) &&
         FMath::IsNearlyEqual(Left.LastRootReducerMilliseconds,
                              Right.LastRootReducerMilliseconds) &&
         Left.LastProjectedEntityCount == Right.LastProjectedEntityCount &&
         Left.LastProjectedComponentTypeCount ==
             Right.LastProjectedComponentTypeCount;
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
         Left.LastTownspersonInteractionRequest ==
             Right.LastTownspersonInteractionRequest &&
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
