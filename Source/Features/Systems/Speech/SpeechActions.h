#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/SpeechTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpeechActions {

inline const rtk::ActionCreator<FSpeechPayload> &SpeechStarted() {
  static const func::Lazy<rtk::ActionCreator<FSpeechPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSpeechPayload> {
        return rtk::createAction<FSpeechPayload>(
            TEXT("systems/speech/started"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FSpeechPayload> &SpeechStopped() {
  static const func::Lazy<rtk::ActionCreator<FSpeechPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSpeechPayload> {
        return rtk::createAction<FSpeechPayload>(
            TEXT("systems/speech/stopped"));
      });
  return func::eval(Creator);
}

} // namespace SpeechActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
