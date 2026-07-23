#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/SystemsDialogueTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace DialogueActions {

/**
 * @fn inline const rtk::ActionCreator<FDialoguePayload> &DialogueObserved()
 * @brief RTK action creator for a dialogue system observation event.
 * User Story: As a features systems dialogue consumer, I need to invoke dialogue observed through a stable signature so the features systems dialogue workflow remains explicit and composable.
 */
inline const rtk::ActionCreator<FDialoguePayload> &DialogueObserved() {
  static const func::Lazy<rtk::ActionCreator<FDialoguePayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FDialoguePayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FDialoguePayload>(
            TEXT("systems/dialogue/observed"));
      });
  return func::eval(Creator);
}

} // namespace DialogueActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
