#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/SystemsSpeechTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpeechActions {

inline const rtk::ActionCreator<FSpeechPayload> &SpeechStarted() {
  static const func::Lazy<rtk::ActionCreator<FSpeechPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSpeechPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FSpeechPayload>(
            TEXT("systems/speech/started"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FSpeechPayload> &SpeechStopped() {
  static const func::Lazy<rtk::ActionCreator<FSpeechPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSpeechPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FSpeechPayload>(
            TEXT("systems/speech/stopped"));
      });
  return func::eval(Creator);
}

} // namespace SpeechActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
