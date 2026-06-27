#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueActions.h"
#include "Features/Systems/Dialogue/DialogueReducers.h"
#include "Features/Systems/Dialogue/DialogueSelectors.h"
#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueSlice {

inline FDialogueState CreateInitialState() {
  return (func::pipe(FDialogueState{}) |
          [](FDialogueState State) -> FDialogueState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FDialogueState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FDialogueState>> Slice =
      func::lazy([]() -> rtk::Slice<FDialogueState> {
        return rtk::createSlice<FDialogueState>(
            TEXT("systems/dialogue"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FDialogueState> &Builder) {
              Builder.addCase(DialogueActions::DialogueObserved(),
                              DialogueReducers::ReduceDialogueObserved);
            });
      });
  return func::eval(Slice);
}

} // namespace DialogueSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
