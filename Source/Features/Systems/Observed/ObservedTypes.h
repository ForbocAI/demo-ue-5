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

/** User Story: As a features systems observed consumer, I need to compare values for equality through a stable signature so the features systems observed workflow remains explicit and composable. @fn inline bool operator==(const FSystemsPayload &Left, const FSystemsPayload &Right) */
inline bool operator==(const FSystemsPayload &Left,
                       const FSystemsPayload &Right) {
  return Left.Id == Right.Id;
}

/** User Story: As a features systems observed consumer, I need to compare values for inequality through a stable signature so the features systems observed workflow remains explicit and composable. @fn inline bool operator!=(const FSystemsPayload &Left, const FSystemsPayload &Right) */
inline bool operator!=(const FSystemsPayload &Left,
                       const FSystemsPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems observed consumer, I need to compare values for equality through a stable signature so the features systems observed workflow remains explicit and composable. @fn inline bool operator==(const FSystemsState &Left, const FSystemsState &Right) */
inline bool operator==(const FSystemsState &Left,
                       const FSystemsState &Right) {
  return Left.bReady == Right.bReady &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

/** User Story: As a features systems observed consumer, I need to compare values for inequality through a stable signature so the features systems observed workflow remains explicit and composable. @fn inline bool operator!=(const FSystemsState &Left, const FSystemsState &Right) */
inline bool operator!=(const FSystemsState &Left,
                       const FSystemsState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
