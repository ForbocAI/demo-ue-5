#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FDialogueRuntimeSettings {
  FString ReplyPayloadIdFormat;
};


inline bool operator==(const FDialogueRuntimeSettings &Left,
                       const FDialogueRuntimeSettings &Right) {
  return Left.ReplyPayloadIdFormat == Right.ReplyPayloadIdFormat;
}

inline bool operator!=(const FDialogueRuntimeSettings &Left,
                       const FDialogueRuntimeSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
