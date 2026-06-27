#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueSelectors {

inline bool SelectReady(const FDialogueState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString> SelectLastActionId(
    const FDialogueState &State) {
  return State.LastActionId;
}

} // namespace DialogueSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
