#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/SystemsInteractionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace InteractionActions {

/**
 * @brief RTK action creator for observed townsperson interaction candidates.
 */
inline const rtk::ActionCreator<FCandidatesObserved> &
TownspersonCandidatesObserved() {
  static const func::Lazy<rtk::ActionCreator<FCandidatesObserved>>
      Creator = func::lazy(
          []() -> rtk::ActionCreator<FCandidatesObserved> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FCandidatesObserved>(
                TEXT("systems/interaction/townspersonCandidatesObserved"));
          });
  return func::eval(Creator);
}

} // namespace InteractionActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
