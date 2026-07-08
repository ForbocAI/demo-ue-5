#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Store {

struct FSettings {
  TArray<FString> DataBackedMapLabels;
  TArray<FString> ReduxLoggerMiddlewareLabels;
  TArray<FString> ProjectionGateLabels;
  FString ReduxLoggerCategory;
  FString ReduxLoggerActionTitlePrefix;
  FString TerrainEntity;
  FString TerrainProjectionDomain;
};

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.DataBackedMapLabels == Right.DataBackedMapLabels &&
         Left.ReduxLoggerMiddlewareLabels ==
             Right.ReduxLoggerMiddlewareLabels &&
         Left.ProjectionGateLabels == Right.ProjectionGateLabels &&
         Left.ReduxLoggerCategory == Right.ReduxLoggerCategory &&
         Left.ReduxLoggerActionTitlePrefix ==
             Right.ReduxLoggerActionTitlePrefix &&
         Left.TerrainEntity == Right.TerrainEntity &&
         Left.TerrainProjectionDomain == Right.TerrainProjectionDomain;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Store
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
