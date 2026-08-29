#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FMeshSettings {
  float TerrainVertexHeightOffset;
  int32 TerrainMaterialSlotIndex;
  int32 TerrainMeshSectionIndex;
  bool bTerrainCreateCollision;
};

/** User Story: As a level geometry consumer, I need to compare terrain mesh settings so generated mesh payloads remain deterministic. @fn inline bool operator==(const FMeshSettings &Left, const FMeshSettings &Right) */
inline bool operator==(const FMeshSettings &Left,
                       const FMeshSettings &Right) {
  return FMath::IsNearlyEqual(Left.TerrainVertexHeightOffset,
                              Right.TerrainVertexHeightOffset) &&
         Left.TerrainMaterialSlotIndex == Right.TerrainMaterialSlotIndex &&
         Left.TerrainMeshSectionIndex == Right.TerrainMeshSectionIndex &&
         Left.bTerrainCreateCollision == Right.bTerrainCreateCollision;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
