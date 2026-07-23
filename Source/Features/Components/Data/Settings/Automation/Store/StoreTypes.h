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

/** User Story: As a settings automation store consumer, I need to compare values for equality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator==(const FLabels &Left, const FLabels &Right) */
inline bool operator==(const FLabels &Left, const FLabels &Right) {
  return Left.DataBackedMapLabels == Right.DataBackedMapLabels &&
         Left.ReduxLoggerMiddlewareLabels ==
             Right.ReduxLoggerMiddlewareLabels &&
         Left.ProjectionGateLabels == Right.ProjectionGateLabels;
}

/** User Story: As a settings automation store consumer, I need to compare values for inequality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator!=(const FLabels &Left, const FLabels &Right) */
inline bool operator!=(const FLabels &Left, const FLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation store consumer, I need to compare values for equality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator==(const FLogger &Left, const FLogger &Right) */
inline bool operator==(const FLogger &Left, const FLogger &Right) {
  return Left.ReduxLoggerCategory == Right.ReduxLoggerCategory &&
         Left.ReduxLoggerActionTitlePrefix ==
             Right.ReduxLoggerActionTitlePrefix;
}

/** User Story: As a settings automation store consumer, I need to compare values for inequality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator!=(const FLogger &Left, const FLogger &Right) */
inline bool operator!=(const FLogger &Left, const FLogger &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation store consumer, I need to compare values for equality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator==(const FTerrain &Left, const FTerrain &Right) */
inline bool operator==(const FTerrain &Left, const FTerrain &Right) {
  return Left.TerrainEntity == Right.TerrainEntity &&
         Left.TerrainProjectionDomain == Right.TerrainProjectionDomain;
}

/** User Story: As a settings automation store consumer, I need to compare values for inequality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator!=(const FTerrain &Left, const FTerrain &Right) */
inline bool operator!=(const FTerrain &Left, const FTerrain &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation store consumer, I need to compare values for equality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator==(const FSettings &Left, const FSettings &Right) */
inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Labels == Right.Labels && Left.Logger == Right.Logger &&
         Left.Terrain == Right.Terrain;
}

/** User Story: As a settings automation store consumer, I need to compare values for inequality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator!=(const FSettings &Left, const FSettings &Right) */
inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Store
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
