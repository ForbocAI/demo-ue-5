#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/LevelTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeLevelViewPayloadRequest {
  const FLevelTerrainData *TerrainData = nullptr;
  const FLevelOrthoData *OrthoData = nullptr;
  const FLevelRuntimeLayoutSeed *RuntimeLayout = nullptr;
  const ForbocAI::Game::Data::FLevelGeometrySettings *Geometry = nullptr;
};

struct FRuntimeLevelViewPayload {
  bool bTerrainMeshLoaded = false;
  FTerrainMeshPayload TerrainMesh;
  TArray<FLevelRuntimeSectionSpawn> Sections;
  TArray<FRuntimeTownspersonViewSpawn> Townspeople;
  TArray<FRuntimeHorseViewSpawn> Horses;
};

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

} // namespace Level
} // namespace Game
} // namespace ForbocAI
