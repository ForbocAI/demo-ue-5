#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"
#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeTownspersonIdentity {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
};

struct FRuntimeTownspersonConversation {
  FString InteractionPrompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
};

struct FRuntimeTownspersonViewSpawn {
  FRuntimeTownspersonIdentity Identity;
  FRuntimeTownspersonConversation Conversation;
  TArray<FVector> PatrolRoute;
  FLevelDistanceLodStage Lod;
};

struct FRuntimeHorseViewSpawn {
  FString Name;
  TArray<FVector> PatrolRoute;
  bool bMountedRider = false;
  FLevelDistanceLodStage Lod;
};

struct FRuntimeTownspersonViewSpawnRequest {
  FTownspersonSeed Seed;
  const FLevelTerrainData *TerrainData = nullptr;
};

struct FRuntimeHorseViewSpawnRequest {
  FHorseRouteSeed Seed;
  const FLevelTerrainData *TerrainData = nullptr;
};

/** User Story: As a level presentation spawn consumer, I need to compare values for equality through a stable signature so the level presentation spawn workflow remains explicit and composable. @fn inline bool operator==(const FRuntimeTownspersonViewSpawn &Left, const FRuntimeTownspersonViewSpawn &Right) */
inline bool operator==(const FRuntimeTownspersonViewSpawn &Left,
                       const FRuntimeTownspersonViewSpawn &Right) {
  return Left.Identity.Id == Right.Identity.Id &&
         Left.Identity.Name == Right.Identity.Name &&
         Left.Identity.Role == Right.Identity.Role &&
         Left.Identity.Persona == Right.Identity.Persona &&
         Left.Conversation.InteractionPrompt ==
             Right.Conversation.InteractionPrompt &&
         Left.Conversation.DefaultPlayerLine ==
             Right.Conversation.DefaultPlayerLine &&
         Left.Conversation.PinnedResponse ==
             Right.Conversation.PinnedResponse &&
         Left.PatrolRoute == Right.PatrolRoute && Left.Lod == Right.Lod;
}

/** User Story: As a level presentation spawn consumer, I need to compare values for inequality through a stable signature so the level presentation spawn workflow remains explicit and composable. @fn inline bool operator!=(const FRuntimeTownspersonViewSpawn &Left, const FRuntimeTownspersonViewSpawn &Right) */
inline bool operator!=(const FRuntimeTownspersonViewSpawn &Left,
                       const FRuntimeTownspersonViewSpawn &Right) {
  return !(Left == Right);
}

/** User Story: As a level presentation spawn consumer, I need to compare values for equality through a stable signature so the level presentation spawn workflow remains explicit and composable. @fn inline bool operator==(const FRuntimeHorseViewSpawn &Left, const FRuntimeHorseViewSpawn &Right) */
inline bool operator==(const FRuntimeHorseViewSpawn &Left,
                       const FRuntimeHorseViewSpawn &Right) {
  return Left.Name == Right.Name && Left.PatrolRoute == Right.PatrolRoute &&
         Left.bMountedRider == Right.bMountedRider && Left.Lod == Right.Lod;
}

/** User Story: As a level presentation spawn consumer, I need to compare values for inequality through a stable signature so the level presentation spawn workflow remains explicit and composable. @fn inline bool operator!=(const FRuntimeHorseViewSpawn &Left, const FRuntimeHorseViewSpawn &Right) */
inline bool operator!=(const FRuntimeHorseViewSpawn &Left,
                       const FRuntimeHorseViewSpawn &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
