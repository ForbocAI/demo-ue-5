#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/InteractionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace InteractionSelectors {

inline const FInteractionSelection &
SelectSelectedCandidate(const FInteractionState &State) {
  return State.SelectedCandidate;
}

inline float SelectTownspersonInteractionDistance(
    const FInteractionState &State) {
  return State.TownspersonMaxDistance;
}

inline TArray<FInteractionCandidate>
SelectLastCandidates(const FInteractionState &State) {
  return State.LastCandidates;
}

} // namespace InteractionSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
