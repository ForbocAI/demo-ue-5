#pragma once

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace PlayerSelectors {

inline bool SelectReady(const FPlayerState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

inline func::Maybe<FString>
SelectLastActionId(const FPlayerState &State) {
  return State.LastActionId;
}

inline FRotator
SelectYawRotation(const FPlayerMovementInputRequest &Request) {
  return FRotator(0.0f, Request.ControlRotation.Yaw, 0.0f);
}

inline FPlayerMovementInputViewModel SelectMovementInput(
    const FPlayerMovementInputRequest &Request) {
  const FRotator YawRotation = SelectYawRotation(Request);
  FPlayerMovementInputViewModel Model;
  Model.ForwardDirection =
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  Model.RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
  Model.ForwardScale = Request.Forward;
  Model.RightScale = Request.Right;
  Model.bShouldMove = Request.bControllerAvailable;
  return Model;
}

} // namespace PlayerSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
