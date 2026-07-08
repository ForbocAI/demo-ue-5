#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FDialogueSettings {
  FString ReplyPayloadIdFormat;
};


inline bool operator==(const FDialogueSettings &Left,
                       const FDialogueSettings &Right) {
  return Left.ReplyPayloadIdFormat == Right.ReplyPayloadIdFormat;
}

inline bool operator!=(const FDialogueSettings &Left,
                       const FDialogueSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
