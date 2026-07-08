#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/SystemsInteractionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace InteractionSelectors {

/**
 * @brief Selector for the reducer-owned selected candidate.
 */
inline const FInteractionSelection &
SelectSelectedCandidate(const FInteractionState &State) {
  return State.SelectedCandidate;
}

/**
 * @brief Selector for the configured townsperson interaction distance.
 */
inline float SelectTownspersonInteractionDistance(
    const FInteractionState &State) {
  return State.TownspersonMaxDistance;
}

/**
 * @brief Selector for the observed candidate payload.
 */
inline TArray<FInteractionCandidate>
SelectCandidates(const FInteractionState &State) {
  return State.Candidates;
}

} // namespace InteractionSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
