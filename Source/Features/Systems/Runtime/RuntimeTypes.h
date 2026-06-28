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
#include "Features/Systems/Spawn/SpawnTypes.h"
#include "Features/Systems/Speech/SpeechTypes.h"
#include "Features/Systems/SystemsTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

/**
 * @brief ECS projection owned by the runtime root state.
 *
 * Architecture: RTK owns the store, ECS owns neutral entity/component
 * projection, and functional-core helpers remain pure composition utilities.
 */
struct FRuntimeEcsState {
  ecs::FWorld World = ecs::createWorld();
};

/**
 * @brief Store-derived townsperson spawn data consumed by runtime views.
 */
struct FRuntimeTownspersonViewSpawn {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
  TArray<FVector> PatrolRoute;
};

/**
 * @brief Store-derived mounted-horse spawn data consumed by runtime views.
 */
struct FRuntimeHorseViewSpawn {
  FString Name;
  TArray<FVector> PatrolRoute;
  bool bMountedRider = false;
};

/**
 * @brief Request object for reducing one townsperson seed into view spawn data.
 */
struct FRuntimeTownspersonViewSpawnRequest {
  FTownspersonSeed Seed;
  const FLevelTerrainData *TerrainData = nullptr;
};

/**
 * @brief Request object for reducing one horse seed into view spawn data.
 */
struct FRuntimeHorseViewSpawnRequest {
  FHorseRouteSeed Seed;
  const FLevelTerrainData *TerrainData = nullptr;
};

/**
 * @brief Request object for reducing full runtime level presentation payloads.
 */
struct FRuntimeLevelViewPayloadRequest {
  const FLevelTerrainData *TerrainData = nullptr;
  const FLevelOrthoData *OrthoData = nullptr;
  const FLevelRuntimeLayoutSeed *RuntimeLayout = nullptr;
  const ForbocAI::Demo::Data::FLevelGeometrySettings *Geometry = nullptr;
};

/**
 * @brief Reducer-owned dialogue request selected from a townsperson source.
 */
struct FRuntimeTownspersonInteractionRequest {
  FString Name;
  FString Role;
  FString Persona;
  FString PlayerLine;
  FString PinnedResponse;
};

/**
 * @brief UE-observed interaction source lowered into RTK payload data.
 */
struct FRuntimeTownspersonInteractionSource {
  FString Name;
  FString Role;
  FString Persona;
  FString DefaultPlayerLine;
  FString PinnedResponse;
};

/**
 * @brief Thunk result that carries dialogue and UI payloads together.
 */
struct FRuntimeTownspersonInteractionPayload {
  FDialogueReplyPayload DialogueReply;
  FUIPayload UI;
};

/**
 * @brief Store-derived level payload used by ARuntimeLevelView.
 */
struct FRuntimeLevelViewPayload {
  bool bTerrainMeshLoaded = false;
  FTerrainMeshPayload TerrainMesh;
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
  return Left.Id == Right.Id && Left.Name == Right.Name && Left.Role == Right.Role &&
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

inline bool operator==(const FRuntimeTownspersonInteractionSource &Left,
                       const FRuntimeTownspersonInteractionSource &Right) {
  return Left.Name == Right.Name && Left.Role == Right.Role &&
         Left.Persona == Right.Persona &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse;
}

inline bool operator!=(const FRuntimeTownspersonInteractionSource &Left,
                       const FRuntimeTownspersonInteractionSource &Right) {
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
         Left.Sections == Right.Sections &&
         Left.Townspeople == Right.Townspeople && Left.Horses == Right.Horses;
}

inline bool operator!=(const FRuntimeLevelViewPayload &Left,
                       const FRuntimeLevelViewPayload &Right) {
  return !(Left == Right);
}

/**
 * @brief Root state for the single UE demo store.
 *
 * Architecture: RuntimeSlice composes every feature slice into this state.
 * Views dispatch actions/thunks and read selectors; reducers own mutations and
 * ECS projection keeps entity/component data inspectable without replacing RTK.
 */
struct FRuntimeState {
  FRuntimeEcsState Ecs;
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

inline bool operator==(const FRuntimeState &Left,
                       const FRuntimeState &Right) {
  return Left.Ecs == Right.Ecs &&
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
} // namespace Demo
} // namespace ForbocAI
