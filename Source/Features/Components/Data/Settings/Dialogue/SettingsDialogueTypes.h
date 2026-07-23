#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FDialogueSettings {
  FString ReplyPayloadIdFormat;
};


/** User Story: As a data settings dialogue consumer, I need to compare values for equality through a stable signature so the data settings dialogue workflow remains explicit and composable. @fn inline bool operator==(const FDialogueSettings &Left, const FDialogueSettings &Right) */
inline bool operator==(const FDialogueSettings &Left,
                       const FDialogueSettings &Right) {
  return Left.ReplyPayloadIdFormat == Right.ReplyPayloadIdFormat;
}

/** User Story: As a data settings dialogue consumer, I need to compare values for inequality through a stable signature so the data settings dialogue workflow remains explicit and composable. @fn inline bool operator!=(const FDialogueSettings &Left, const FDialogueSettings &Right) */
inline bool operator!=(const FDialogueSettings &Left,
                       const FDialogueSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
