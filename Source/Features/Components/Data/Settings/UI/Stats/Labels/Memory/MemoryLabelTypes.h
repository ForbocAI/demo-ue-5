#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FMemoryLabelSettings {
  FString UsedPhysicalMemoryLabel;
  FString PeakPhysicalMemoryLabel;
  FString UsedVirtualMemoryLabel;
};

/** User Story: As a stats label consumer, I need to compare memory labels so immutable settings transitions remain explicit. @fn inline bool operator==(const FMemoryLabelSettings &Left, const FMemoryLabelSettings &Right) */
inline bool operator==(const FMemoryLabelSettings &Left,
                       const FMemoryLabelSettings &Right) {
  return Left.UsedPhysicalMemoryLabel == Right.UsedPhysicalMemoryLabel &&
         Left.PeakPhysicalMemoryLabel == Right.PeakPhysicalMemoryLabel &&
         Left.UsedVirtualMemoryLabel == Right.UsedVirtualMemoryLabel;
}

/** User Story: As a stats label consumer, I need to compare memory labels for inequality so immutable settings transitions remain explicit. @fn inline bool operator!=(const FMemoryLabelSettings &Left, const FMemoryLabelSettings &Right) */
inline bool operator!=(const FMemoryLabelSettings &Left,
                       const FMemoryLabelSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
