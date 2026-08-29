#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotScheduleSettings {
  float ObservationIntervalSeconds;
  float InitialObservationTimeSeconds;
  float PatrolTickIntervalSeconds;
  float InitialPatrolPauseRemainingSeconds;
};

/** User Story: As a bot settings consumer, I need to compare schedule settings so authored observation and patrol timing remains deterministic. @fn inline bool operator==(const FBotScheduleSettings &Left, const FBotScheduleSettings &Right) */
inline bool operator==(const FBotScheduleSettings &Left,
                       const FBotScheduleSettings &Right) {
  return FMath::IsNearlyEqual(Left.ObservationIntervalSeconds,
                              Right.ObservationIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.InitialObservationTimeSeconds,
                              Right.InitialObservationTimeSeconds) &&
         FMath::IsNearlyEqual(Left.PatrolTickIntervalSeconds,
                              Right.PatrolTickIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.InitialPatrolPauseRemainingSeconds,
                              Right.InitialPatrolPauseRemainingSeconds);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
