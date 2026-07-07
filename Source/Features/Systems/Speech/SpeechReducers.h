#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/SpeechAdaptersReducers.h"
#include "Features/Systems/Speech/SpeechTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpeechReducers {

inline FSpeechState ReduceRuntimeSettings(
    const FSpeechState &State,
    const ForbocAI::Game::Data::FSpeechRuntimeSettings &Settings) {
  return (func::pipe(State) |
          [&Settings](FSpeechState Next) -> FSpeechState {
            Next.RuntimeSettings = Settings;
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
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.LastSpokenText = func::just(Action.PayloadValue.Text);
            Next.ActivePhonemes = SpeechOps::EstimatePhonemesFromText(
                Action.PayloadValue.Text, State.RuntimeSettings);
            Next.CurrentViseme = SpeechOps::RestViseme(State.RuntimeSettings);
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
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.ActivePhonemes.Empty();
            Next.CurrentViseme = SpeechOps::RestViseme(State.RuntimeSettings);
            Next.bSpeaking = false;
            return Next;
          })
      .val;
}

} // namespace SpeechReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
