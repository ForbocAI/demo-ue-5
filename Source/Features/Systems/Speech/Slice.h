#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/Adapters/Slice.h"
#include "Features/Systems/Speech/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpeechReducers {

inline FSpeechState ReduceSettings(
    const FSpeechState &State,
    const ForbocAI::Game::Data::FSpeechSettings &Settings) {
  return (func::pipe(State) |
          [&Settings](FSpeechState Next) -> FSpeechState {
            Next.Settings = Settings;
            Next.CurrentViseme = SpeechOps::RestViseme(Settings);
            Next.bSpeaking = Settings.bInitialSpeechActive;
            return Next;
          })
      .val;
}

inline FSpeechState ReduceSpeechStarted(
    const FSpeechState &State,
    const rtk::PayloadAction<FSpeechPayload> &Action) {
  return (func::pipe(State) |
          [&State, &Action](FSpeechState Next) -> FSpeechState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.SpokenText = func::just(Action.PayloadValue.Text);
            Next.ActivePhonemes = SpeechOps::EstimatePhonemesFromText(
                Action.PayloadValue.Text, State.Settings);
            Next.CurrentViseme = SpeechOps::RestViseme(State.Settings);
            Next.bSpeaking = true;
            return Next;
          })
      .val;
}

inline FSpeechState ReduceSpeechStopped(
    const FSpeechState &State,
    const rtk::PayloadAction<FSpeechPayload> &Action) {
  return (func::pipe(State) |
          [&State, &Action](FSpeechState Next) -> FSpeechState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.ActivePhonemes.Empty();
            Next.CurrentViseme = SpeechOps::RestViseme(State.Settings);
            Next.bSpeaking = false;
            return Next;
          })
      .val;
}

} // namespace SpeechReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Speech/Actions.h"
#include "Features/Systems/Speech/Selectors.h"
#include "Features/Systems/Speech/Types.h"

namespace ForbocAI {
namespace Game {
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
            State.ActionId = func::nothing<FString>();
            State.SpokenText = func::nothing<FString>();
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
} // namespace Game
} // namespace ForbocAI
