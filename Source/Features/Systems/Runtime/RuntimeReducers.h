#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeReducers {

FRuntimeState ReduceRuntimeEcsProjected(const FRuntimeState &State);

inline FRuntimeState ReduceRuntimeHydrated(
    const FRuntimeState &State, const rtk::PayloadAction<FRuntimeState> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState) -> FRuntimeState {
            return ReduceRuntimeEcsProjected(Action.PayloadValue);
          })
      .val;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
