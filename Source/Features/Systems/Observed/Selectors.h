#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Observed/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsSelectors {

inline bool SelectReady(const FSystemsState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString> SelectActionId(
    const FSystemsState &State) {
  return State.ActionId;
}

} // namespace SystemsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
