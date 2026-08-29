#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FHistorySettings {
  FString ChatMessageFormat;
  FString HistoryRoleSeparator;
  int32 HistoryTextStartOffset;
  int32 HistoryMinimumRoleIndex;
};

/** User Story: As a UI settings consumer, I need to compare history settings so chat projection remains deterministic. @fn inline bool operator==(const FHistorySettings &Left, const FHistorySettings &Right) */
inline bool operator==(const FHistorySettings &Left,
                       const FHistorySettings &Right) {
  return Left.ChatMessageFormat == Right.ChatMessageFormat &&
         Left.HistoryRoleSeparator == Right.HistoryRoleSeparator &&
         Left.HistoryTextStartOffset == Right.HistoryTextStartOffset &&
         Left.HistoryMinimumRoleIndex == Right.HistoryMinimumRoleIndex;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
