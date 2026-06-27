#pragma once

#include "Core/rtk.hpp"

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FPlayerPayload {
  FString Id;
};

struct FPlayerState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  bool bReady = false;
};

inline bool operator==(const FPlayerPayload &Left,
                       const FPlayerPayload &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FPlayerPayload &Left,
                       const FPlayerPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPlayerState &Left,
                       const FPlayerState &Right) {
  return Left.bReady == Right.bReady &&
         Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value);
}

inline bool operator!=(const FPlayerState &Left,
                       const FPlayerState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
