#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotCorePayload {
  FString Id;
};

struct FBotCoreState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  bool bReady;
};

inline bool operator==(const FBotCoreState &Left,
                       const FBotCoreState &Right) {
  return Left.bReady == Right.bReady &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

inline bool operator!=(const FBotCoreState &Left,
                       const FBotCoreState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
