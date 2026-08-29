#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FTime {
  float TimeOfDayHour;
};

inline bool operator==(const FTime &Left, const FTime &Right) {
  return FMath::IsNearlyEqual(Left.TimeOfDayHour, Right.TimeOfDayHour);
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
