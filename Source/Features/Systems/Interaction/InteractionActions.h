#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/SystemsInteractionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace InteractionActions {

/**
 * @fn inline const rtk::ActionCreator<FCandidatesObserved> & TownspersonCandidatesObserved()
 * @brief RTK action creator for observed townsperson interaction candidates.
 * User Story: As a features systems interaction consumer, I need to invoke townsperson candidates observed through a stable signature so the features systems interaction workflow remains explicit and composable.
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
