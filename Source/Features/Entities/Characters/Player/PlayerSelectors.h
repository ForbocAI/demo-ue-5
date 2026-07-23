#pragma once

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/CharactersPlayerTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace PlayerSelectors {

/** User Story: As a entities characters player consumer, I need to invoke select ready through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline bool SelectReady(const FPlayerState &State) */
inline bool SelectReady(const FPlayerState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/** User Story: As a entities characters player consumer, I need to invoke select action id through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline func::Maybe<FString> SelectActionId(const FPlayerState &State) */
inline func::Maybe<FString>
SelectActionId(const FPlayerState &State) {
  return State.ActionId;
}

/** User Story: As a entities characters player consumer, I need to invoke select presentation through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline const FPresentationViewModel & SelectPresentation(const FPlayerState &State) */
inline const FPresentationViewModel &
SelectPresentation(const FPlayerState &State) {
  return State.Presentation;
}

} // namespace PlayerSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
