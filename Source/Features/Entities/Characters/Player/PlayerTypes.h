#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FPlayerPayload {
  FString Id;
};

struct FPlayerMovementInputRequest {
  FRotator ControlRotation = FRotator::ZeroRotator;
  float Right = 0.0f;
  float Forward = 0.0f;
  bool bControllerAvailable = false;
};

struct FPlayerMovementInputViewModel {
  FVector ForwardDirection = FVector::ZeroVector;
  FVector RightDirection = FVector::ZeroVector;
  float ForwardScale = 0.0f;
  float RightScale = 0.0f;
  bool bShouldMove = false;
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

inline bool operator==(const FPlayerMovementInputRequest &Left,
                       const FPlayerMovementInputRequest &Right) {
  return Left.ControlRotation == Right.ControlRotation &&
         FMath::IsNearlyEqual(Left.Right, Right.Right) &&
         FMath::IsNearlyEqual(Left.Forward, Right.Forward) &&
         Left.bControllerAvailable == Right.bControllerAvailable;
}

inline bool operator!=(const FPlayerMovementInputRequest &Left,
                       const FPlayerMovementInputRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPlayerMovementInputViewModel &Left,
                       const FPlayerMovementInputViewModel &Right) {
  return Left.ForwardDirection == Right.ForwardDirection &&
         Left.RightDirection == Right.RightDirection &&
         FMath::IsNearlyEqual(Left.ForwardScale, Right.ForwardScale) &&
         FMath::IsNearlyEqual(Left.RightScale, Right.RightScale) &&
         Left.bShouldMove == Right.bShouldMove;
}

inline bool operator!=(const FPlayerMovementInputViewModel &Left,
                       const FPlayerMovementInputViewModel &Right) {
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
