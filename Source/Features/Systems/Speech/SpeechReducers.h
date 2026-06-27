#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/SpeechTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpeechReducers {

inline FSpeechState ReduceSpeechStarted(
    const FSpeechState &State,
    const rtk::PayloadAction<FSpeechPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FSpeechState Next) -> FSpeechState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.LastSpokenText = func::just(Action.PayloadValue.Text);
            Next.ActivePhonemes =
                SpeechOps::EstimatePhonemesFromText(Action.PayloadValue.Text);
            Next.CurrentViseme = FVisemeMapping{TEXT("viseme_sil"), 0.0f};
            Next.bSpeaking = true;
            return Next;
          })
      .val;
}

inline FSpeechState ReduceSpeechStopped(
    const FSpeechState &State,
    const rtk::PayloadAction<FSpeechPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FSpeechState Next) -> FSpeechState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.ActivePhonemes.Empty();
            Next.CurrentViseme = FVisemeMapping{TEXT("viseme_sil"), 0.0f};
            Next.bSpeaking = false;
            return Next;
          })
      .val;
}

} // namespace SpeechReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
