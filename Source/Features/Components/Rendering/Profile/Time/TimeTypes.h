#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FTime {
  float TimeOfDayHour;
};

/** User Story: As the rendering profile state owner, I need time-of-day values compared structurally so unchanged cadence does not schedule a redundant transition. @fn inline bool operator==(const FTime &Left, const FTime &Right) */
inline bool operator==(const FTime &Left, const FTime &Right) {
  return FMath::IsNearlyEqual(Left.TimeOfDayHour, Right.TimeOfDayHour);
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
