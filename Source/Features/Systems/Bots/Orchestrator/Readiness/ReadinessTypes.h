#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotOrchestratorReadinessPayload {
  FString Id;
};

struct FBotOrchestratorReadinessState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  bool bReady;
};

/** User Story: As a bots orchestrator readiness consumer, I need to compare values for equality through a stable signature so the bots orchestrator readiness workflow remains explicit and composable. @fn inline bool operator==(const FBotOrchestratorReadinessState &Left, const FBotOrchestratorReadinessState &Right) */
inline bool operator==(const FBotOrchestratorReadinessState &Left,
                       const FBotOrchestratorReadinessState &Right) {
  return Left.bReady == Right.bReady &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

/** User Story: As a bots orchestrator readiness consumer, I need to compare values for inequality through a stable signature so the bots orchestrator readiness workflow remains explicit and composable. @fn inline bool operator!=(const FBotOrchestratorReadinessState &Left, const FBotOrchestratorReadinessState &Right) */
inline bool operator!=(const FBotOrchestratorReadinessState &Left,
                       const FBotOrchestratorReadinessState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
