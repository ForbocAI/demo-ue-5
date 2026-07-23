#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Lifecycle/LifecycleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLifecycleSlice {

/** User Story: As a features systems lifecycle consumer, I need to invoke create initial state through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState CreateInitialState() */
FRuntimeLifecycleState CreateInitialState();

/**
 * @fn const rtk::Slice<FRuntimeLifecycleState> &GetSlice()
 * @brief Slice that reduces the runtime bootstrap async-thunk lifecycle into a
 * loading status. Uses builder.addCase on the createAsyncThunk pending/
 * fulfilled/rejected action creators exposed by RuntimeThunks.
 * User Story: As a features systems lifecycle consumer, I need to invoke get slice through a stable signature so the features systems lifecycle workflow remains explicit and composable.
 */
const rtk::Slice<FRuntimeLifecycleState> &GetSlice();

} // namespace RuntimeLifecycleSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
