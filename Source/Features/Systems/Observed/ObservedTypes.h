#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FSystemsPayload {
  FString Id;
};

struct FSystemsState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  bool bReady = false;
};

inline bool operator==(const FSystemsPayload &Left,
                       const FSystemsPayload &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FSystemsPayload &Left,
                       const FSystemsPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSystemsState &Left,
                       const FSystemsState &Right) {
  return Left.bReady == Right.bReady &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

inline bool operator!=(const FSystemsState &Left,
                       const FSystemsState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
