#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FBotCorePayload {
  FString Id;
};

struct FBotCoreState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  bool bReady = false;
};

inline bool operator==(const FBotCoreState &Left,
                       const FBotCoreState &Right) {
  return Left.bReady == Right.bReady &&
         Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value);
}

inline bool operator!=(const FBotCoreState &Left,
                       const FBotCoreState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
