#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSlice {

/**
 * @brief Returns the root RTK slice composed into Store::ConfigureStore.
 */
const rtk::Slice<FRuntimeState> &GetSlice();

/**
 * @brief Public thunk re-export for player spawn loading.
 */
rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn();

/**
 * @brief Public thunk re-export for runtime level view payload loading.
 */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();

} // namespace RuntimeSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
