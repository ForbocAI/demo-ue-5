#pragma once

#include "Features/Components/Data/Settings/UI/Stats/Threshold/Performance/ThresholdPerformanceTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Threshold/Resource/ResourceTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FThresholdSettings {
  FPerformanceThresholdSettings Performance;
  FResourceThresholdSettings Resource;
};

/** User Story: As a stats overlay consumer, I need one composed threshold concern so performance and resource severity share an immutable boundary. @fn inline bool operator==(const FThresholdSettings &Left, const FThresholdSettings &Right) */
inline bool operator==(const FThresholdSettings &Left,
                       const FThresholdSettings &Right) {
  return Left.Performance == Right.Performance &&
         Left.Resource == Right.Resource;
}

/** User Story: As a stats overlay consumer, I need to compare composed threshold settings for inequality so immutable transitions remain explicit. @fn inline bool operator!=(const FThresholdSettings &Left, const FThresholdSettings &Right) */
inline bool operator!=(const FThresholdSettings &Left,
                       const FThresholdSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
