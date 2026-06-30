#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/UI/UIActions.h"
#include "Features/Systems/UI/UIReducers.h"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace UISlice {

inline FUIState CreateInitialState() {
  return (func::pipe(FUIState{}) |
          [](FUIState State) -> FUIState {
            State.LastActionId = func::nothing<FString>();
            State.bConversationVisible = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FUIState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FUIState>> Slice =
      func::lazy([]() -> rtk::Slice<FUIState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FUIState>(
            TEXT("systems/ui"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FUIState> &Builder) {
              Builder.addCase(UIActions::ConversationPresented(),
                              UIReducers::ReduceConversationPresented);
              Builder.addCase(UIActions::ChatHistoryRendered(),
                              UIReducers::ReduceChatHistoryRendered);
            });
      });
  return func::eval(Slice);
}

} // namespace UISlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
