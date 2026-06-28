#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/InteractionTypes.h"

namespace ForbocAI {
namespace Demo {
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
 * @brief Selector for the last observed candidate payload.
 */
inline TArray<FInteractionCandidate>
SelectLastCandidates(const FInteractionState &State) {
  return State.LastCandidates;
}

} // namespace InteractionSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
