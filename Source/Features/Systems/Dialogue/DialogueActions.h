#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueActions {

/**
 * @brief RTK action creator for a dialogue system observation event.
 */
inline const rtk::ActionCreator<FDialoguePayload> &DialogueObserved() {
  static const func::Lazy<rtk::ActionCreator<FDialoguePayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FDialoguePayload> {
        return rtk::createAction<FDialoguePayload>(
            TEXT("systems/dialogue/observed"));
      });
  return func::eval(Creator);
}

} // namespace DialogueActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
