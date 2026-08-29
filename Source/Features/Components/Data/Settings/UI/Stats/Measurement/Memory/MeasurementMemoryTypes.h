#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FMemoryMetricSettings {
  int32 EmptyMemoryMegabytes;
  int32 MemoryBytesPerMegabyte;
};

/** User Story: As a stats measurement consumer, I need to compare memory metric settings so unit conversion and empty observations remain deterministic. @fn inline bool operator==(const FMemoryMetricSettings &Left, const FMemoryMetricSettings &Right) */
inline bool operator==(const FMemoryMetricSettings &Left,
                       const FMemoryMetricSettings &Right) {
  return Left.EmptyMemoryMegabytes == Right.EmptyMemoryMegabytes &&
         Left.MemoryBytesPerMegabyte == Right.MemoryBytesPerMegabyte;
}

/** User Story: As a stats measurement consumer, I need to compare memory metric settings for inequality so unit conversion and empty observations remain deterministic. @fn inline bool operator!=(const FMemoryMetricSettings &Left, const FMemoryMetricSettings &Right) */
inline bool operator!=(const FMemoryMetricSettings &Left,
                       const FMemoryMetricSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
