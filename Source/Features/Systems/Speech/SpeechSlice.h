#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/Phoneme/PhonemeSlice.h"
#include "Features/Systems/Speech/SystemsSpeechTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpeechReducers {

/** User Story: As a features systems speech consumer, I need to invoke reduce settings through a stable signature so the features systems speech workflow remains explicit and composable. @fn inline FSpeechState ReduceSettings( const FSpeechState &State, const ForbocAI::Game::Data::FSpeechSettings &Settings) */
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

/** User Story: As a features systems speech consumer, I need to invoke reduce speech started through a stable signature so the features systems speech workflow remains explicit and composable. @fn inline FSpeechState ReduceSpeechStarted( const FSpeechState &State, const rtk::PayloadAction<FSpeechPayload> &Action) */
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

/** User Story: As a features systems speech consumer, I need to invoke reduce speech stopped through a stable signature so the features systems speech workflow remains explicit and composable. @fn inline FSpeechState ReduceSpeechStopped( const FSpeechState &State, const rtk::PayloadAction<FSpeechPayload> &Action) */
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

#include "Features/Systems/Speech/SpeechActions.h"
#include "Features/Systems/Speech/SpeechSelectors.h"
#include "Features/Systems/Speech/SystemsSpeechTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpeechSlice {

/**
 * @fn inline FSpeechState CreateInitialState()
 * @brief Builds the initial speech RTK slice state.
 * @return Speech state with no active text, no viseme, and not speaking.
 *
 * User story: As a speech presenter, phoneme and viseme state should start
 * deterministic before audio/thunk effects dispatch reducer actions.
 * User Story: As a features systems speech consumer, I need to invoke create initial state through a stable signature so the features systems speech workflow remains explicit and composable.
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
 * @fn inline const rtk::Slice<FSpeechState> &GetSlice()
 * @brief Returns the lazily constructed speech RTK slice.
 * @return The singleton slice that owns speech reducer registration.
 *
 * User story: As speech integration code, start/stop actions should reduce
 * through one slice so views and ECS projections observe consistent state.
 * User Story: As a features systems speech consumer, I need to invoke get slice through a stable signature so the features systems speech workflow remains explicit and composable.
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
