#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeInteractionTypes.h"
#include "Features/Systems/Runtime/RuntimeLevelViewTypes.h"
#include "Features/Systems/Runtime/RuntimeStateTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeThunks {

/**
 * @brief createAsyncThunk type prefixes for the runtime bootstrap workflows.
 * Shared so reducers/listeners can react to the pending/fulfilled/rejected
 * lifecycle actions without duplicating the prefix strings.
 */
FString RequestPlayerSpawnTypePrefix();
FString RequestLevelViewPayloadTypePrefix();

/**
 * @brief The runtime bootstrap createAsyncThunk configs, exposed so slices can
 * react to their pending/fulfilled/rejected lifecycle action creators via
 * builder.addCase (the canonical RTK extraReducers pattern).
 */
const rtk::AsyncThunkConfig<FSpawnPointPayload, rtk::FEmptyPayload,
                            FRuntimeState> &
RequestPlayerSpawnAsyncThunk();
const rtk::AsyncThunkConfig<FRuntimeLevelViewPayload, rtk::FEmptyPayload,
                            FRuntimeState> &
RequestLevelViewPayloadAsyncThunk();

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
} // namespace Game
} // namespace ForbocAI
