#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Speech/SpeechActions.h"
#include "Features/Systems/Speech/SpeechReducers.h"
#include "Features/Systems/Speech/SpeechSelectors.h"
#include "Features/Systems/Speech/SpeechTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpeechSlice {

/**
 * @brief Builds the initial speech RTK slice state.
 * @signature inline FSpeechState CreateInitialState()
 * @return Speech state with no active text, no viseme, and not speaking.
 *
 * User story: As a speech presenter, phoneme and viseme state should start
 * deterministic before audio/thunk effects dispatch reducer actions.
 */
inline FSpeechState CreateInitialState() {
  return (func::pipe(FSpeechState{}) |
          [](FSpeechState State) -> FSpeechState {
            State.LastActionId = func::nothing<FString>();
            State.LastSpokenText = func::nothing<FString>();
            State.CurrentViseme = FVisemeMapping{};
            State.bSpeaking = false;
            return State;
          })
      .val;
}

/**
 * @brief Returns the lazily constructed speech RTK slice.
 * @signature inline const rtk::Slice<FSpeechState> &GetSlice()
 * @return The singleton slice that owns speech reducer registration.
 *
 * User story: As speech integration code, start/stop actions should reduce
 * through one slice so views and ECS projections observe consistent state.
 */
inline const rtk::Slice<FSpeechState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FSpeechState>> Slice =
      func::lazy([]() -> rtk::Slice<FSpeechState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FSpeechState>(
            TEXT("systems/speech"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FSpeechState> &Builder) {
              Builder.addCase(SpeechActions::SpeechStarted(),
                              SpeechReducers::ReduceSpeechStarted);
              Builder.addCase(SpeechActions::SpeechStopped(),
                              SpeechReducers::ReduceSpeechStopped);
            });
      });
  return func::eval(Slice);
}

} // namespace SpeechSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
