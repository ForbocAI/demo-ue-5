#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"
#include "Features/Systems/Terrain/SystemsTerrainTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeLevelViewPayloadRequest {
  const FLevelTerrainData *TerrainData = nullptr;
  const FLevelOrthoData *OrthoData = nullptr;
  const FLayoutSeed *Layout = nullptr;
  const ForbocAI::Game::Data::FGeometrySettings *Geometry = nullptr;
};

struct FRuntimeLevelViewPayload {
  bool bTerrainMeshLoaded = false;
  FMeshPayload TerrainMesh;
  TArray<FSectionSpawn> Sections;
  TArray<FRuntimeTownspersonViewSpawn> Townspeople;
  TArray<FRuntimeHorseViewSpawn> Horses;
};

/** User Story: As a systems level presentation consumer, I need to compare values for equality through a stable signature so the systems level presentation workflow remains explicit and composable. @fn inline bool operator==(const FRuntimeLevelViewPayload &Left, const FRuntimeLevelViewPayload &Right) */
inline bool operator==(const FRuntimeLevelViewPayload &Left,
                       const FRuntimeLevelViewPayload &Right) {
  return Left.bTerrainMeshLoaded == Right.bTerrainMeshLoaded &&
         Left.TerrainMesh == Right.TerrainMesh &&
         Left.Sections == Right.Sections &&
         Left.Townspeople == Right.Townspeople && Left.Horses == Right.Horses;
}

/** User Story: As a systems level presentation consumer, I need to compare values for inequality through a stable signature so the systems level presentation workflow remains explicit and composable. @fn inline bool operator!=(const FRuntimeLevelViewPayload &Left, const FRuntimeLevelViewPayload &Right) */
inline bool operator!=(const FRuntimeLevelViewPayload &Left,
                       const FRuntimeLevelViewPayload &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
