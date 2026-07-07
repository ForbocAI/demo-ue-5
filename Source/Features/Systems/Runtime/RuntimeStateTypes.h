#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"
#include "Features/Systems/Bots/AI/BotAITypes.h"
#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Bots/Core/BotCoreTypes.h"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorTypes.h"
#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesTypes.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"
#include "Features/Systems/Dialogue/DialogueTypes.h"
#include "Features/Systems/Interaction/InteractionTypes.h"
#include "Features/Systems/Level/LevelTypes.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"
#include "Features/Systems/Nature/NatureTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Runtime/RuntimeLifecycleTypes.h"
#include "Features/Systems/Spawn/SpawnTypes.h"
#include "Features/Systems/Speech/SpeechTypes.h"
#include "Features/Systems/SystemsTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"
#include "Features/Systems/UI/UITypes.h"

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
  ForbocAI::Game::Data::FRuntimeObservationIdSettings RuntimeObservationIds;
  ForbocAI::Game::Data::FRuntimeDebugMessageSettings RuntimeDebugMessages;
  ForbocAI::Game::Data::FRuntimeViewNameSettings RuntimeViewNames;
  ForbocAI::Game::Data::FRuntimeTextSettings RuntimeText;
  ForbocAI::Game::Data::FBotRuntimeSettings BotRuntime;
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
  return Left.RuntimeObservationIds == Right.RuntimeObservationIds &&
         Left.RuntimeDebugMessages == Right.RuntimeDebugMessages &&
         Left.RuntimeViewNames == Right.RuntimeViewNames &&
         Left.RuntimeText == Right.RuntimeText &&
         Left.BotRuntime == Right.BotRuntime &&
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
