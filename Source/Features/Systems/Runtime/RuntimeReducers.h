#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeInteractionTypes.h"
#include "Features/Systems/Runtime/RuntimeLevelViewTypes.h"
#include "Features/Systems/Runtime/RuntimeStateTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"

#include "Features/Systems/Runtime/RuntimeInteractionReducers.h"
#include "Features/Systems/Runtime/RuntimeProjectionReducers.h"
#include "Features/Systems/Runtime/RuntimeViewReducers.h"

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
