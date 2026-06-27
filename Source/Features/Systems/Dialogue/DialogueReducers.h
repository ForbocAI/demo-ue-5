#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueReducers {

inline FDialogueState ReduceDialogueObserved(
    const FDialogueState &State,
    const rtk::PayloadAction<FDialoguePayload> &Action) {
  return (func::pipe(State) |
          [&Action](FDialogueState Next) -> FDialogueState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace DialogueReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
