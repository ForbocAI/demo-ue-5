#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Store {

struct FLabels {
  TArray<FString> DataBackedMapLabels;
  TArray<FString> ReduxLoggerMiddlewareLabels;
  TArray<FString> ProjectionGateLabels;
};

struct FLogger {
  FString ReduxLoggerCategory;
  FString ReduxLoggerActionTitlePrefix;
};

struct FTerrain {
  FString TerrainEntity;
  FString TerrainProjectionDomain;
};

struct FSettings {
  FLabels Labels;
  FLogger Logger;
  FTerrain Terrain;
};

inline bool operator==(const FLabels &Left, const FLabels &Right) {
  return Left.DataBackedMapLabels == Right.DataBackedMapLabels &&
         Left.ReduxLoggerMiddlewareLabels ==
             Right.ReduxLoggerMiddlewareLabels &&
         Left.ProjectionGateLabels == Right.ProjectionGateLabels;
}

inline bool operator!=(const FLabels &Left, const FLabels &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLogger &Left, const FLogger &Right) {
  return Left.ReduxLoggerCategory == Right.ReduxLoggerCategory &&
         Left.ReduxLoggerActionTitlePrefix ==
             Right.ReduxLoggerActionTitlePrefix;
}

inline bool operator!=(const FLogger &Left, const FLogger &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTerrain &Left, const FTerrain &Right) {
  return Left.TerrainEntity == Right.TerrainEntity &&
         Left.TerrainProjectionDomain == Right.TerrainProjectionDomain;
}

inline bool operator!=(const FTerrain &Left, const FTerrain &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Labels == Right.Labels && Left.Logger == Right.Logger &&
         Left.Terrain == Right.Terrain;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Store
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
