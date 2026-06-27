#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/SystemsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsReducers {

inline FSystemsState
ReduceSystemsObserved(const FSystemsState &State,
                      const rtk::PayloadAction<FSystemsPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FSystemsState Next) -> FSystemsState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace SystemsReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
