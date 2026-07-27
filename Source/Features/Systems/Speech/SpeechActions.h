#pragma once
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Core/rtk.hpp"
#include "Features/Systems/Speech/SystemsSpeechTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpeechActions {

/** User Story: As a features systems speech consumer, I need to invoke speech started through a stable signature so the features systems speech workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FSpeechPayload> &SpeechStarted() */
inline const rtk::ActionCreator<FSpeechPayload> &SpeechStarted() {
  static const func::Lazy<rtk::ActionCreator<FSpeechPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSpeechPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FSpeechPayload>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGVE5ADF44584BB));
      });
  return func::eval(Creator);
}

/** User Story: As a features systems speech consumer, I need to invoke speech stopped through a stable signature so the features systems speech workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FSpeechPayload> &SpeechStopped() */
inline const rtk::ActionCreator<FSpeechPayload> &SpeechStopped() {
  static const func::Lazy<rtk::ActionCreator<FSpeechPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSpeechPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FSpeechPayload>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGVD596F7C3B724));
      });
  return func::eval(Creator);
}

} // namespace SpeechActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
