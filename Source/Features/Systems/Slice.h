#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Level/View/Types.h"
#include "Features/Systems/State/Types.h"
#include "Features/Systems/View/Spawn/Types.h"

#include "Features/Systems/Interaction/Conversation/Slice.h"
#include "Features/Systems/Projection/Slice.h"
#include "Features/Systems/View/Slice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

FRuntimeState ReduceRuntimeAction(const FRuntimeState &State,
                                  const rtk::AnyAction &Action);

/**
 * @brief Case reducer for RuntimeActions::RuntimeHydrated.
 */
inline FRuntimeState ReduceRuntimeHydrated(
    const FRuntimeState &State, const rtk::PayloadAction<FRuntimeState> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState) -> FRuntimeState {
            return ReduceRuntimeProjected(Action.PayloadValue);
          })
      .val;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Level/View/Types.h"
#include "Features/Systems/State/Types.h"
#include "Features/Systems/View/Spawn/Types.h"

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

#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Level/View/Types.h"
#include "Features/Systems/State/Types.h"
#include "Features/Systems/View/Spawn/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeFactories {

/**
 * @brief Creates the initial root state by composing feature initial states.
 */
FRuntimeState CreateInitialState();

} // namespace RuntimeFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
