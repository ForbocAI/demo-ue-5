#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeThunks {

/**
 * @brief RTK thunk that loads runtime data and resolves the player spawn.
 */
rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn();

/**
 * @brief RTK thunk that seeds data and resolves the full runtime level view
 * payload.
 */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();

} // namespace RuntimeThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
