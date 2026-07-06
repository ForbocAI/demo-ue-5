#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/InteractionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace InteractionActions {

/**
 * @brief RTK action creator for observed townsperson interaction candidates.
 */
inline const rtk::ActionCreator<FInteractionCandidatesObserved> &
TownspersonCandidatesObserved() {
  static const func::Lazy<rtk::ActionCreator<FInteractionCandidatesObserved>>
      Creator = func::lazy(
          []() -> rtk::ActionCreator<FInteractionCandidatesObserved> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FInteractionCandidatesObserved>(
                TEXT("systems/interaction/townspersonCandidatesObserved"));
          });
  return func::eval(Creator);
}

} // namespace InteractionActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
