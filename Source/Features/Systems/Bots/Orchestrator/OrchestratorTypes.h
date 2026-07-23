#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotOrchestratorPayload {
  FString Id;
};

struct FBotOrchestratorState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  bool bReady;
};

/** User Story: As a systems bots orchestrator consumer, I need to compare values for equality through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn inline bool operator==(const FBotOrchestratorState &Left, const FBotOrchestratorState &Right) */
inline bool operator==(const FBotOrchestratorState &Left,
                       const FBotOrchestratorState &Right) {
  return Left.bReady == Right.bReady &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

/** User Story: As a systems bots orchestrator consumer, I need to compare values for inequality through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn inline bool operator!=(const FBotOrchestratorState &Left, const FBotOrchestratorState &Right) */
inline bool operator!=(const FBotOrchestratorState &Left,
                       const FBotOrchestratorState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
