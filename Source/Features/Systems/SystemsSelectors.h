#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/SystemsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsSelectors {

inline bool SelectReady(const FSystemsState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString> SelectLastActionId(
    const FSystemsState &State) {
  return State.LastActionId;
}

} // namespace SystemsSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
