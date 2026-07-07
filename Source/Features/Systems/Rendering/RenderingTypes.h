#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Rendering/RenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/RenderingTextureTypes.h"
#include "Features/Components/Rendering/RenderingProfileTypes.h"

class UMaterialInterface;
class UStaticMeshComponent;
class UWorld;

#include "Features/Systems/Rendering/RenderingPayloadTypes.h"
#include "Features/Systems/Rendering/RenderingPresentationTypes.h"
#include "Features/Systems/Rendering/RenderingStatsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTextureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRenderingState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  TArray<FLevelDistanceLodStage> DistanceLodStages;
  ForbocAI::Game::Data::FRenderingRuntimeSettings RuntimeSettings;
  FRenderingAssetPaths AssetPaths;
  FTownspersonPresentationViewModel TownspersonPresentation;
  FHorsePresentationViewModel HorsePresentation;
  FRuntimeStatsPresentationModel StatsPresentation;
  double StatsRefreshElapsedSeconds = 0.0;
  double PolyCountRefreshElapsedSeconds = 0.0;
  double LastFrameClockSeconds = 0.0;
  double BudgetLogLastSeconds = 0.0;
  double BudgetScreenshotLastSeconds = 0.0;
  int32 BudgetScreenshotIndex = 0;
  int64 CachedPolyCount = 0;
  double CachedPolyCountMilliseconds = 0.0;
  bool bReady = false;
};

inline bool operator==(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value) &&
         Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.DistanceLodStages == Right.DistanceLodStages &&
         Left.RuntimeSettings == Right.RuntimeSettings &&
         Left.AssetPaths == Right.AssetPaths &&
         Left.TownspersonPresentation == Right.TownspersonPresentation &&
         Left.HorsePresentation == Right.HorsePresentation &&
         Left.StatsPresentation == Right.StatsPresentation &&
         FMath::IsNearlyEqual(Left.StatsRefreshElapsedSeconds, Right.StatsRefreshElapsedSeconds) &&
         FMath::IsNearlyEqual(Left.PolyCountRefreshElapsedSeconds, Right.PolyCountRefreshElapsedSeconds) &&
         FMath::IsNearlyEqual(Left.LastFrameClockSeconds, Right.LastFrameClockSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetLogLastSeconds, Right.BudgetLogLastSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetScreenshotLastSeconds, Right.BudgetScreenshotLastSeconds) &&
         Left.BudgetScreenshotIndex == Right.BudgetScreenshotIndex &&
         Left.CachedPolyCount == Right.CachedPolyCount &&
         FMath::IsNearlyEqual(Left.CachedPolyCountMilliseconds, Right.CachedPolyCountMilliseconds) &&
         Left.bReady == Right.bReady;
}

inline bool operator!=(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
