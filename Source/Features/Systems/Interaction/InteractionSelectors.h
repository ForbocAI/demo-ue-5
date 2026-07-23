#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/SystemsInteractionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace InteractionSelectors {

/**
 * @fn inline const FSelection & SelectSelectedCandidate(const FInteractionState &State)
 * @brief Selector for the reducer-owned selected candidate.
 * User Story: As a features systems interaction consumer, I need to invoke select selected candidate through a stable signature so the features systems interaction workflow remains explicit and composable.
 */
inline const FSelection &
SelectSelectedCandidate(const FInteractionState &State) {
  return State.SelectedCandidate;
}

/**
 * @fn inline float SelectTownspersonInteractionDistance( const FInteractionState &State)
 * @brief Selector for the configured townsperson interaction distance.
 * User Story: As a features systems interaction consumer, I need to invoke select townsperson interaction distance through a stable signature so the features systems interaction workflow remains explicit and composable.
 */
inline float SelectTownspersonInteractionDistance(
    const FInteractionState &State) {
  return State.TownspersonMaxDistance;
}

/**
 * @fn inline TArray<FCandidate> SelectCandidates(const FInteractionState &State)
 * @brief Selector for the observed candidate payload.
 * User Story: As a features systems interaction consumer, I need to invoke select candidates through a stable signature so the features systems interaction workflow remains explicit and composable.
 */
inline TArray<FCandidate>
SelectCandidates(const FInteractionState &State) {
  return State.Candidates;
}

} // namespace InteractionSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
