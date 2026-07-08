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

struct FRenderingState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  TArray<FLevelDistanceLodStage> DistanceLodStages;
  ForbocAI::Game::Data::FRenderingSettings Settings;
  FRenderingAssetPaths AssetPaths;
  FTownspersonPresentationViewModel TownspersonPresentation;
  FHorsePresentationViewModel HorsePresentation;
  FRuntimeStatsPresentationModel StatsPresentation;
  double StatsRefreshElapsedSeconds = 0.0;
  double PolyCountRefreshElapsedSeconds = 0.0;
  double FrameClockSeconds = 0.0;
  double BudgetLogPreviousSeconds = 0.0;
  double BudgetScreenshotPreviousSeconds = 0.0;
  int32 BudgetScreenshotIndex = 0;
  int64 CachedPolyCount = 0;
  double CachedPolyCountMilliseconds = 0.0;
  bool bReady = false;
};

inline bool operator==(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value) &&
         Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.DistanceLodStages == Right.DistanceLodStages &&
         Left.Settings == Right.Settings &&
         Left.AssetPaths == Right.AssetPaths &&
         Left.TownspersonPresentation == Right.TownspersonPresentation &&
         Left.HorsePresentation == Right.HorsePresentation &&
         Left.StatsPresentation == Right.StatsPresentation &&
         FMath::IsNearlyEqual(Left.StatsRefreshElapsedSeconds, Right.StatsRefreshElapsedSeconds) &&
         FMath::IsNearlyEqual(Left.PolyCountRefreshElapsedSeconds, Right.PolyCountRefreshElapsedSeconds) &&
         FMath::IsNearlyEqual(Left.FrameClockSeconds, Right.FrameClockSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetLogPreviousSeconds, Right.BudgetLogPreviousSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetScreenshotPreviousSeconds, Right.BudgetScreenshotPreviousSeconds) &&
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
