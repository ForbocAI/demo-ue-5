#pragma once

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace PlayerReducers {

inline FPlayerState
ReducePlayerObserved(const FPlayerState &State,
                     const rtk::PayloadAction<FPlayerPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FPlayerState Next) -> FPlayerState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace PlayerReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
