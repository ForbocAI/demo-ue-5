#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotPipelinePayload {
  FString Id;
};

struct FBotPipelineState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  bool bReady;
};

/** User Story: As a systems bots pipeline consumer, I need to compare values for equality through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline bool operator==(const FBotPipelineState &Left, const FBotPipelineState &Right) */
inline bool operator==(const FBotPipelineState &Left,
                       const FBotPipelineState &Right) {
  return Left.bReady == Right.bReady &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

/** User Story: As a systems bots pipeline consumer, I need to compare values for inequality through a stable signature so the systems bots pipeline workflow remains explicit and composable. @fn inline bool operator!=(const FBotPipelineState &Left, const FBotPipelineState &Right) */
inline bool operator!=(const FBotPipelineState &Left,
                       const FBotPipelineState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
