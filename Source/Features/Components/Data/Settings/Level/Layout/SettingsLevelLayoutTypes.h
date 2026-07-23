#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FTerrainLayoutSourceSettings {
  FString TerrainBlocksJsonPath;
  FString TerrainLabelsJsonPath;
};

struct FTownLayoutSourceSettings {
  FString TownBlocksJsonPath;
  FString TownLabelsJsonPath;
};

struct FMineLayoutSourceSettings {
  FString MineBlocksJsonPath;
  FString MineLabelsJsonPath;
};

struct FOverlayLayoutSourceSettings {
  FString OverlayLabelsJsonPath;
};

struct FEntitySourceSettings {
  FString LandmarksJsonPath;
  FString TownspeopleJsonPath;
  FString HorsesJsonPath;
  FString NatureJsonPath;
};

struct FDataSourceSettings {
  FTerrainLayoutSourceSettings Terrain;
  FTownLayoutSourceSettings Town;
  FMineLayoutSourceSettings Mine;
  FOverlayLayoutSourceSettings Overlay;
  FEntitySourceSettings Entities;
};

/** User Story: As a level layout consumer, I need to compare source settings for equality through a stable signature so authored data ownership remains explicit and composable. @fn inline bool operator==(const FDataSourceSettings &Left, const FDataSourceSettings &Right) */
inline bool operator==(const FDataSourceSettings &Left,
                       const FDataSourceSettings &Right) {
  return Left.Terrain.TerrainBlocksJsonPath ==
             Right.Terrain.TerrainBlocksJsonPath &&
         Left.Terrain.TerrainLabelsJsonPath ==
             Right.Terrain.TerrainLabelsJsonPath &&
         Left.Town.TownBlocksJsonPath == Right.Town.TownBlocksJsonPath &&
         Left.Town.TownLabelsJsonPath == Right.Town.TownLabelsJsonPath &&
         Left.Mine.MineBlocksJsonPath == Right.Mine.MineBlocksJsonPath &&
         Left.Mine.MineLabelsJsonPath == Right.Mine.MineLabelsJsonPath &&
         Left.Overlay.OverlayLabelsJsonPath ==
             Right.Overlay.OverlayLabelsJsonPath &&
         Left.Entities.LandmarksJsonPath ==
             Right.Entities.LandmarksJsonPath &&
         Left.Entities.TownspeopleJsonPath ==
             Right.Entities.TownspeopleJsonPath &&
         Left.Entities.HorsesJsonPath == Right.Entities.HorsesJsonPath &&
         Left.Entities.NatureJsonPath == Right.Entities.NatureJsonPath;
}

/** User Story: As a level layout consumer, I need to compare source settings for inequality through a stable signature so authored data ownership remains explicit and composable. @fn inline bool operator!=(const FDataSourceSettings &Left, const FDataSourceSettings &Right) */
inline bool operator!=(const FDataSourceSettings &Left,
                       const FDataSourceSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
