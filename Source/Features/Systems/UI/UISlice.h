#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/UI/UIActions.h"
#include "Features/Systems/UI/Chat/Rendering/ChatRenderingSlice.h"
#include "Features/Systems/UI/Chat/ChatSlice.h"
#include "Features/Systems/UI/Conversation/UIConversationSlice.h"
#include "Features/Systems/UI/SystemsUITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UISlice {

/**
 * @fn inline FUIState CreateInitialState()
 * @brief Builds the initial UI RTK slice state.
 * @return UI state with no last action and the conversation surface hidden.
 *
 * User story: As a player entering the runtime, conversation UI should start from
 * deterministic reducer-owned state before ECS and view events project into it.
 * User Story: As a features systems ui consumer, I need to invoke create initial state through a stable signature so the features systems ui workflow remains explicit and composable.
 */
inline FUIState CreateInitialState() {
  return (func::pipe(FUIState{}) |
          [](FUIState State) -> FUIState {
            State.ActionId = func::nothing<FString>();
            State.bConversationVisible = false;
            return State;
          })
      .val;
}

/**
 * @fn inline const rtk::Slice<FUIState> &GetSlice()
 * @brief Returns the lazily constructed UI RTK slice.
 * @return The singleton slice that owns UI reducer registration.
 *
 * User story: As gameplay UI code, dispatching conversation actions should
 * flow through one RTK slice so views read selector-derived state only.
 * User Story: As a features systems ui consumer, I need to invoke get slice through a stable signature so the features systems ui workflow remains explicit and composable.
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
