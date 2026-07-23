#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FSourceSettings {
  FString TerrainCsvPath;
  FString OrthoCsvPath;
};

/** User Story: As a level terrain settings consumer, I need to compare terrain sources through a stable signature so authored source changes remain deterministic. @fn inline bool operator==(const FSourceSettings &Left, const FSourceSettings &Right) */
inline bool operator==(const FSourceSettings &Left,
                       const FSourceSettings &Right) {
  return Left.TerrainCsvPath == Right.TerrainCsvPath &&
         Left.OrthoCsvPath == Right.OrthoCsvPath;
}

/** User Story: As a level terrain settings consumer, I need to compare terrain sources for inequality through a stable signature so authored source changes remain observable. @fn inline bool operator!=(const FSourceSettings &Left, const FSourceSettings &Right) */
inline bool operator!=(const FSourceSettings &Left,
                       const FSourceSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
