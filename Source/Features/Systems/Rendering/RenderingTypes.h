#pragma once
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Rendering/Distance/Lod/ComponentsRenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"
#include "Features/Components/Rendering/Profile/Render/RenderTypes.h"

class UMaterialInterface;
class UStaticMeshComponent;
class UWorld;

#include "Features/Systems/Rendering/Payload/PayloadTypes.h"
#include "Features/Systems/Rendering/Presentation/RenderingPresentationTypes.h"
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
  double StatsRefreshElapsedSeconds = FORBOCAI_DEMOUE5_AUTHORED_NUMBERV3FC4AA7B1C98;
  double PolyCountRefreshElapsedSeconds = FORBOCAI_DEMOUE5_AUTHORED_NUMBERV3FC4AA7B1C98;
  double FrameClockSeconds = FORBOCAI_DEMOUE5_AUTHORED_NUMBERV3FC4AA7B1C98;
};

struct FBudgetClockState {
  double BudgetLogPreviousSeconds = FORBOCAI_DEMOUE5_AUTHORED_NUMBERV3FC4AA7B1C98;
  double BudgetScreenshotPreviousSeconds = FORBOCAI_DEMOUE5_AUTHORED_NUMBERV3FC4AA7B1C98;
  int32 BudgetScreenshotIndex = FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA;
};

struct FPolyCacheState {
  int64 CachedPolyCount = FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA;
  double CachedPolyCountMilliseconds = FORBOCAI_DEMOUE5_AUTHORED_NUMBERV3FC4AA7B1C98;
};

struct FRenderingState {
  FProfileState Profile;
  FCatalogState Catalog;
  FPresentationState Presentation;
  FStatsClockState StatsClock;
  FBudgetClockState BudgetClock;
  FPolyCacheState PolyCache;
};

/** User Story: As a features systems rendering consumer, I need to compare values for equality through a stable signature so the features systems rendering workflow remains explicit and composable. @fn inline bool operator==(const FRenderingState &Left, const FRenderingState &Right) */
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

/** User Story: As a features systems rendering consumer, I need to compare values for inequality through a stable signature so the features systems rendering workflow remains explicit and composable. @fn inline bool operator!=(const FRenderingState &Left, const FRenderingState &Right) */
inline bool operator!=(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
