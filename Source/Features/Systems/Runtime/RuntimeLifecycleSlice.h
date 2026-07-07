#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeLifecycleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLifecycleSlice {

FRuntimeLifecycleState CreateInitialState();

/**
 * @brief Slice that reduces the runtime bootstrap async-thunk lifecycle into a
 * loading status. Uses builder.addCase on the createAsyncThunk pending/
 * fulfilled/rejected action creators exposed by RuntimeThunks.
 */
const rtk::Slice<FRuntimeLifecycleState> &GetSlice();

} // namespace RuntimeLifecycleSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
