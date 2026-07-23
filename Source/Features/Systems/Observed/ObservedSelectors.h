#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Observed/ObservedTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsSelectors {

/** User Story: As a features systems observed consumer, I need to invoke select ready through a stable signature so the features systems observed workflow remains explicit and composable. @fn inline bool SelectReady(const FSystemsState &State) */
inline bool SelectReady(const FSystemsState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/** User Story: As a features systems observed consumer, I need to invoke select action id through a stable signature so the features systems observed workflow remains explicit and composable. @fn inline func::Maybe<FString> SelectActionId( const FSystemsState &State) */
inline func::Maybe<FString> SelectActionId(
    const FSystemsState &State) {
  return State.ActionId;
}

} // namespace SystemsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
