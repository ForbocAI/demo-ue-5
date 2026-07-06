#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Pipeline/BotPipelineActions.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineReducers.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineSelectors.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPipelineSlice {

inline FBotPipelineState CreateInitialState() {
  return (func::pipe(FBotPipelineState{}) |
          [](FBotPipelineState State) -> FBotPipelineState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FBotPipelineState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotPipelineState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotPipelineState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotPipelineState>(
            TEXT("systems/bots/pipeline"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FBotPipelineState>
                   &Builder) {
              Builder.addCase(BotPipelineActions::PipelineObserved(),
                              BotPipelineReducers::ReducePipelineObserved);
            });
      });
  return func::eval(Slice);
}

} // namespace BotPipelineSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
