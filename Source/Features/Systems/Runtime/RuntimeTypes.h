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
#include "Features/Systems/Level/LevelTypes.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"
#include "Features/Systems/Nature/NatureTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Spawn/SpawnTypes.h"
#include "Features/Systems/Speech/SpeechTypes.h"
#include "Features/Systems/SystemsTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FRuntimeEcsState {
  ecs::FWorld World = ecs::createWorld();
};

struct FRuntimeTownspersonViewSpawn {
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
  TArray<FVector> PatrolRoute;
};

struct FRuntimeHorseViewSpawn {
  FString Name;
  TArray<FVector> PatrolRoute;
  bool bMountedRider = false;
};

struct FRuntimeTownspersonViewSpawnRequest {
  FTownspersonSeed Seed;
  const FLevelTerrainData *TerrainData = nullptr;
};

struct FRuntimeHorseViewSpawnRequest {
  FHorseRouteSeed Seed;
  const FLevelTerrainData *TerrainData = nullptr;
};

struct FRuntimeLevelViewPayloadRequest {
  const FLevelTerrainData *TerrainData = nullptr;
  const FLevelOrthoData *OrthoData = nullptr;
  const FLevelRuntimeLayoutSeed *RuntimeLayout = nullptr;
};

struct FRuntimeTownspersonInteractionRequest {
  FString Name;
  FString Role;
  FString Persona;
  FString PlayerLine;
  FString PinnedResponse;
};

struct FRuntimeTownspersonInteractionPayload {
  FDialogueReplyPayload DialogueReply;
  FUIPayload UI;
};

struct FRuntimeLevelViewPayload {
  bool bTerrainMeshLoaded = false;
  FTerrainMeshPayload TerrainMesh;
  FLevelBlockSpawn FallbackTerrainBlock;
  TArray<FLevelRuntimeSectionSpawn> Sections;
  TArray<FRuntimeTownspersonViewSpawn> Townspeople;
  TArray<FRuntimeHorseViewSpawn> Horses;
};

inline bool operator==(const FRuntimeEcsState &Left,
                       const FRuntimeEcsState &Right) {
  return Left.World == Right.World;
}

inline bool operator!=(const FRuntimeEcsState &Left,
                       const FRuntimeEcsState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeTownspersonViewSpawn &Left,
                       const FRuntimeTownspersonViewSpawn &Right) {
  return Left.Name == Right.Name && Left.Role == Right.Role &&
         Left.Persona == Right.Persona &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse &&
         Left.PatrolRoute == Right.PatrolRoute;
}

inline bool operator!=(const FRuntimeTownspersonViewSpawn &Left,
                       const FRuntimeTownspersonViewSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeHorseViewSpawn &Left,
                       const FRuntimeHorseViewSpawn &Right) {
  return Left.Name == Right.Name && Left.PatrolRoute == Right.PatrolRoute &&
         Left.bMountedRider == Right.bMountedRider;
}

inline bool operator!=(const FRuntimeHorseViewSpawn &Left,
                       const FRuntimeHorseViewSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeTownspersonInteractionRequest &Left,
                       const FRuntimeTownspersonInteractionRequest &Right) {
  return Left.Name == Right.Name && Left.Role == Right.Role &&
         Left.Persona == Right.Persona && Left.PlayerLine == Right.PlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse;
}

inline bool operator!=(const FRuntimeTownspersonInteractionRequest &Left,
                       const FRuntimeTownspersonInteractionRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeTownspersonInteractionPayload &Left,
                       const FRuntimeTownspersonInteractionPayload &Right) {
  return Left.DialogueReply == Right.DialogueReply && Left.UI == Right.UI;
}

inline bool operator!=(const FRuntimeTownspersonInteractionPayload &Left,
                       const FRuntimeTownspersonInteractionPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeLevelViewPayload &Left,
                       const FRuntimeLevelViewPayload &Right) {
  return Left.bTerrainMeshLoaded == Right.bTerrainMeshLoaded &&
         Left.TerrainMesh == Right.TerrainMesh &&
         Left.FallbackTerrainBlock == Right.FallbackTerrainBlock &&
         Left.Sections == Right.Sections &&
         Left.Townspeople == Right.Townspeople && Left.Horses == Right.Horses;
}

inline bool operator!=(const FRuntimeLevelViewPayload &Left,
                       const FRuntimeLevelViewPayload &Right) {
  return !(Left == Right);
}

struct FRuntimeState {
  FRuntimeEcsState Ecs;
  FPlayerState Player;
  FSystemsState Systems;
  FLevelSystemState Level;
  FRenderingState Rendering;
  FDialogueState Dialogue;
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

inline bool operator==(const FRuntimeState &Left,
                       const FRuntimeState &Right) {
  return Left.Ecs == Right.Ecs &&
         Left.Player == Right.Player &&
         Left.Terrain == Right.Terrain &&
         Left.Systems == Right.Systems && Left.Level == Right.Level &&
         Left.Rendering == Right.Rendering &&
         Left.Dialogue == Right.Dialogue && Left.Speech == Right.Speech &&
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
} // namespace Demo
} // namespace ForbocAI
