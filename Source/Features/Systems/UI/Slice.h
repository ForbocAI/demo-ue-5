#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/UI/Actions.h"
#include "Features/Systems/UI/Shared/Slice.h"
#include "Features/Systems/UI/Chat/Slice.h"
#include "Features/Systems/UI/Conversation/Slice.h"
#include "Features/Systems/UI/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UISlice {

/**
 * @brief Builds the initial UI RTK slice state.
 * @signature inline FUIState CreateInitialState()
 * @return UI state with no last action and the conversation surface hidden.
 *
 * User story: As a player entering the runtime, conversation UI should start from
 * deterministic reducer-owned state before ECS and view events project into it.
 */
inline FUIState CreateInitialState() {
  return (func::pipe(FUIState{}) |
          [](FUIState State) -> FUIState {
            State.LastActionId = func::nothing<FString>();
            State.bConversationVisible = false;
            return State;
          })
      .val;
}

/**
 * @brief Returns the lazily constructed UI RTK slice.
 * @signature inline const rtk::Slice<FUIState> &GetSlice()
 * @return The singleton slice that owns UI reducer registration.
 *
 * User story: As gameplay UI code, dispatching conversation actions should
 * flow through one RTK slice so views read selector-derived state only.
 */
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
} // namespace Game
} // namespace ForbocAI
