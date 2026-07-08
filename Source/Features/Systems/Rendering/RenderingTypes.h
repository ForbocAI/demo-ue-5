#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Rendering/Distance/Lod/ComponentsRenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"
#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

class UMaterialInterface;
class UStaticMeshComponent;
class UWorld;

#include "Features/Systems/Rendering/Payload/PayloadTypes.h"
#include "Features/Systems/Rendering/Presentation/PresentationTypes.h"
#include "Features/Systems/Rendering/Stats/Data/StatsDataTypes.h"
#include "Features/Systems/Rendering/Texture/SystemsRenderingTextureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FProfileState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  FLevelRetroRenderProfile RuntimeProfile;
  ForbocAI::Game::Data::FRenderingSettings Settings;
  FRenderingAssetPaths AssetPaths;
  bool bReady = false;
};

struct FCatalogState {
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  TArray<FLevelDistanceLodStage> DistanceLodStages;
};

struct FPresentationState {
  FTownspersonPresentationViewModel TownspersonPresentation;
  FHorsePresentationViewModel HorsePresentation;
  FRuntimeStatsPresentationModel StatsPresentation;
};

struct FStatsClockState {
  double StatsRefreshElapsedSeconds = 0.0;
  double PolyCountRefreshElapsedSeconds = 0.0;
  double FrameClockSeconds = 0.0;
};

struct FBudgetClockState {
  double BudgetLogPreviousSeconds = 0.0;
  double BudgetScreenshotPreviousSeconds = 0.0;
  int32 BudgetScreenshotIndex = 0;
};

struct FPolyCacheState {
  int64 CachedPolyCount = 0;
  double CachedPolyCountMilliseconds = 0.0;
};

struct FRenderingState {
  FProfileState Profile;
  FCatalogState Catalog;
  FPresentationState Presentation;
  FStatsClockState StatsClock;
  FBudgetClockState BudgetClock;
  FPolyCacheState PolyCache;
};

inline bool operator==(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return Left.Profile.ActionId.hasValue == Right.Profile.ActionId.hasValue &&
         (!Left.Profile.ActionId.hasValue ||
          Left.Profile.ActionId.value == Right.Profile.ActionId.value) &&
         Left.Profile.RuntimeProfile == Right.Profile.RuntimeProfile &&
         Left.Profile.Settings == Right.Profile.Settings &&
         Left.Profile.AssetPaths == Right.Profile.AssetPaths &&
         Left.Profile.bReady == Right.Profile.bReady &&
         Left.Catalog.TextureCatalog == Right.Catalog.TextureCatalog &&
         Left.Catalog.DistanceLodStages == Right.Catalog.DistanceLodStages &&
         Left.Presentation.TownspersonPresentation ==
             Right.Presentation.TownspersonPresentation &&
         Left.Presentation.HorsePresentation ==
             Right.Presentation.HorsePresentation &&
         Left.Presentation.StatsPresentation ==
             Right.Presentation.StatsPresentation &&
         FMath::IsNearlyEqual(
             Left.StatsClock.StatsRefreshElapsedSeconds,
             Right.StatsClock.StatsRefreshElapsedSeconds) &&
         FMath::IsNearlyEqual(
             Left.StatsClock.PolyCountRefreshElapsedSeconds,
             Right.StatsClock.PolyCountRefreshElapsedSeconds) &&
         FMath::IsNearlyEqual(Left.StatsClock.FrameClockSeconds,
                              Right.StatsClock.FrameClockSeconds) &&
         FMath::IsNearlyEqual(
             Left.BudgetClock.BudgetLogPreviousSeconds,
             Right.BudgetClock.BudgetLogPreviousSeconds) &&
         FMath::IsNearlyEqual(
             Left.BudgetClock.BudgetScreenshotPreviousSeconds,
             Right.BudgetClock.BudgetScreenshotPreviousSeconds) &&
         Left.BudgetClock.BudgetScreenshotIndex ==
             Right.BudgetClock.BudgetScreenshotIndex &&
         Left.PolyCache.CachedPolyCount == Right.PolyCache.CachedPolyCount &&
         FMath::IsNearlyEqual(Left.PolyCache.CachedPolyCountMilliseconds,
                              Right.PolyCache.CachedPolyCountMilliseconds);
}

inline bool operator!=(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
