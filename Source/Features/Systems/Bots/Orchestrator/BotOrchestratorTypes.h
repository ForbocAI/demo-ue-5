#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FBotOrchestratorPayload {
  FString Id;
};

struct FBotOrchestratorState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  bool bReady;
};

inline bool operator==(const FBotOrchestratorState &Left,
                       const FBotOrchestratorState &Right) {
  return Left.bReady == Right.bReady &&
         Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value);
}

inline bool operator!=(const FBotOrchestratorState &Left,
                       const FBotOrchestratorState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
