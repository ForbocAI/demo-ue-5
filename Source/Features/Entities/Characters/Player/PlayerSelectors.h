#pragma once

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace PlayerSelectors {

inline bool SelectReady(const FPlayerState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectLastActionId(const FPlayerState &State) {
  return State.LastActionId;
}

inline const FPlayerPresentationViewModel &
SelectPresentation(const FPlayerState &State) {
  return State.Presentation;
}

} // namespace PlayerSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
