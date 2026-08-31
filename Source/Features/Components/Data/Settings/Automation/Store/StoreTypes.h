#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Store {

struct FLabels {
  TArray<FString> DataBackedMapLabels;
  TArray<FString> IdentityLabels;
  TArray<FString> EntityAdapterLabels;
  TArray<FString> RuntimeStatsSamplingLabels;
  TArray<FString> ReduxLoggerMiddlewareLabels;
  TArray<FString> ProjectionGateLabels;
};

struct FEntityAdapterSettings {
  FString LandmarkEntityDomain;
  FString NatureEntityDomain;
  FString TownspersonEntityDomain;
  FString HorseEntityDomain;
  FString NatureProjectionFixtureJsonPath;
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
  FEntityAdapterSettings EntityAdapters;
  FLogger Logger;
  FTerrain Terrain;
};

/** User Story: As a settings automation store consumer, I need to compare values for equality through a stable signature so the settings automation store workflow remains explicit and composable. @fn inline bool operator==(const FLabels &Left, const FLabels &Right) */
inline bool operator==(const FLabels &Left, const FLabels &Right) {
  return Left.DataBackedMapLabels == Right.DataBackedMapLabels &&
         Left.IdentityLabels == Right.IdentityLabels &&
         Left.EntityAdapterLabels == Right.EntityAdapterLabels &&
         Left.RuntimeStatsSamplingLabels ==
             Right.RuntimeStatsSamplingLabels &&
         Left.ReduxLoggerMiddlewareLabels ==
             Right.ReduxLoggerMiddlewareLabels &&
         Left.ProjectionGateLabels == Right.ProjectionGateLabels;
}

/** User Story: As an entity-adapter automation consumer, I need equality over authored domains and fixture paths so settings changes remain deterministic. @fn inline bool operator==(const FEntityAdapterSettings &Left, const FEntityAdapterSettings &Right) */
inline bool operator==(const FEntityAdapterSettings &Left,
                       const FEntityAdapterSettings &Right) {
  return Left.LandmarkEntityDomain == Right.LandmarkEntityDomain &&
         Left.NatureEntityDomain == Right.NatureEntityDomain &&
         Left.TownspersonEntityDomain == Right.TownspersonEntityDomain &&
         Left.HorseEntityDomain == Right.HorseEntityDomain &&
         Left.NatureProjectionFixtureJsonPath ==
             Right.NatureProjectionFixtureJsonPath;
}

/** User Story: As an entity-adapter automation consumer, I need inequality over authored domains and fixture paths so settings drift remains explicit. @fn inline bool operator!=(const FEntityAdapterSettings &Left, const FEntityAdapterSettings &Right) */
inline bool operator!=(const FEntityAdapterSettings &Left,
                       const FEntityAdapterSettings &Right) {
  return !(Left == Right);
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
  return Left.Labels == Right.Labels &&
         Left.EntityAdapters == Right.EntityAdapters &&
         Left.Logger == Right.Logger &&
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
