#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FSamplingSettings {
  int32 TerrainLodStep;
  int32 TerrainMinimumLodStep;
  int32 TerrainGridTerminalOffset;
  int32 TerrainLodGridPadding;
};

/** User Story: As a level geometry consumer, I need to compare terrain sampling settings so terrain detail remains deterministic. @fn inline bool operator==(const FSamplingSettings &Left, const FSamplingSettings &Right) */
inline bool operator==(const FSamplingSettings &Left,
                       const FSamplingSettings &Right) {
  return Left.TerrainLodStep == Right.TerrainLodStep &&
         Left.TerrainMinimumLodStep == Right.TerrainMinimumLodStep &&
         Left.TerrainGridTerminalOffset ==
             Right.TerrainGridTerminalOffset &&
         Left.TerrainLodGridPadding == Right.TerrainLodGridPadding;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
