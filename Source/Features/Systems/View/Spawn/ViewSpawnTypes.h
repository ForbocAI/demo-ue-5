#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"
#include "Features/Systems/Level/SystemsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeTownspersonViewSpawn {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
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

inline bool operator==(const FRuntimeTownspersonViewSpawn &Left,
                       const FRuntimeTownspersonViewSpawn &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name && Left.Role == Right.Role &&
         Left.Persona == Right.Persona &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse &&
         Left.PatrolRoute == Right.PatrolRoute && Left.Lod == Right.Lod;
}

inline bool operator!=(const FRuntimeTownspersonViewSpawn &Left,
                       const FRuntimeTownspersonViewSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeHorseViewSpawn &Left,
                       const FRuntimeHorseViewSpawn &Right) {
  return Left.Name == Right.Name && Left.PatrolRoute == Right.PatrolRoute &&
         Left.bMountedRider == Right.bMountedRider && Left.Lod == Right.Lod;
}

inline bool operator!=(const FRuntimeHorseViewSpawn &Left,
                       const FRuntimeHorseViewSpawn &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
