#pragma once

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Game {
namespace Level {

/**
 * @brief Load status for a runtime bootstrap workflow.
 */
enum class ERuntimeLoadStatus : uint8 {
  Idle,
  Loading,
  Ready,
  Failed,
};

/**
 * @brief Runtime bootstrap lifecycle state.
 *
 * Reduced from the RequestPlayerSpawn / RequestLevelViewPayload
 * createAsyncThunk lifecycle actions (pending/fulfilled/rejected) so runtime
 * readiness and load failures are observable in the store instead of only in
 * logs. Views may gate presentation on this without owning the workflow.
 */
struct FRuntimeLifecycleState {
  ERuntimeLoadStatus PlayerSpawnStatus = ERuntimeLoadStatus::Idle;
  ERuntimeLoadStatus LevelViewStatus = ERuntimeLoadStatus::Idle;
  FString Error;
};

inline bool operator==(const FRuntimeLifecycleState &Left,
                       const FRuntimeLifecycleState &Right) {
  return Left.PlayerSpawnStatus == Right.PlayerSpawnStatus &&
         Left.LevelViewStatus == Right.LevelViewStatus &&
         Left.Error == Right.Error;
}

inline bool operator!=(const FRuntimeLifecycleState &Left,
                       const FRuntimeLifecycleState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
