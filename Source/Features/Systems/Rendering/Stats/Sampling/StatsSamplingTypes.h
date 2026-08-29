#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/SystemsRenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class ERuntimeStatsSampleMode : uint8 {
  Retain,
  Refresh,
};

enum class ERuntimeStatsEffect : uint8 {
  Present,
  BudgetLog,
  BudgetScreenshot,
};

struct FRuntimeStatsRefreshPlan {
  double BudgetClockSeconds;
  float WallDeltaSeconds;
  ERuntimeStatsSampleMode PolyCountMode;
  ERuntimeStatsSampleMode StatsMode;
};

struct FRuntimeStatsBudgetCheckpoint {
  double PreviousSeconds;
  bool bTriggered;
};

struct FRuntimeStatsScreenshotPlan {
  FRuntimeStatsBudgetCheckpoint Checkpoint;
  int32 Index;
};

struct FRuntimeStatsBudgetPlan {
  FRuntimeStatsBudgetCheckpoint Log;
  FRuntimeStatsScreenshotPlan Screenshot;
  TArray<ERuntimeStatsEffect> Effects;
};

struct FRuntimeStatsSamplingPlan {
  FRuntimeStatsRefreshPlan Refresh;
  FRuntimeStatsBudgetPlan Budget;
};

struct FRuntimeStatsObservation {
  FRuntimePolyCountStats PolyCount;
  func::Maybe<FRuntimeStatsViewModel> Stats;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
