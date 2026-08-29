#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FQuadSettings {
  int32 TerrainQuadIndexReserveCount;
  int32 TerrainQuadColumnStep;
  int32 TerrainQuadRowStep;
};

/** User Story: As a level geometry consumer, I need to compare terrain quad settings so mesh indexing remains deterministic. @fn inline bool operator==(const FQuadSettings &Left, const FQuadSettings &Right) */
inline bool operator==(const FQuadSettings &Left,
                       const FQuadSettings &Right) {
  return Left.TerrainQuadIndexReserveCount ==
             Right.TerrainQuadIndexReserveCount &&
         Left.TerrainQuadColumnStep == Right.TerrainQuadColumnStep &&
         Left.TerrainQuadRowStep == Right.TerrainQuadRowStep;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
