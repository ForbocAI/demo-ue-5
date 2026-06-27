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

inline FSpeechState CreateInitialState() {
  return (func::pipe(FSpeechState{}) |
          [](FSpeechState State) -> FSpeechState {
            State.LastActionId = func::nothing<FString>();
            State.LastSpokenText = func::nothing<FString>();
            State.CurrentViseme = FVisemeMapping{TEXT("viseme_sil"), 0.0f};
            State.bSpeaking = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FSpeechState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FSpeechState>> Slice =
      func::lazy([]() -> rtk::Slice<FSpeechState> {
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
