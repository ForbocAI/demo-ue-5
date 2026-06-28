#pragma once

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace PlayerReducers {

inline FPlayerState
ReducePlayerObserved(const FPlayerState &State,
                     const rtk::PayloadAction<FPlayerPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FPlayerState Next) -> FPlayerState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

inline FRotator
ReduceYawRotation(const FPlayerMovementInputRequest &Request) {
  return FRotator(0.0f, Request.ControlRotation.Yaw, 0.0f);
}

inline FPlayerMovementInputViewModel ReduceMovementInput(
    const FPlayerMovementInputRequest &Request) {
  const FRotator YawRotation = ReduceYawRotation(Request);
  FPlayerMovementInputViewModel Model;
  Model.ForwardDirection =
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  Model.RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
  Model.ForwardScale = Request.Forward;
  Model.RightScale = Request.Right;
  Model.bShouldMove = Request.bControllerAvailable;
  return Model;
}

inline FPlayerPresentationViewModel ReducePlayerPresentation(
    const FPlayerPresentationRequest &Request) {
  (void)Request;
  FPlayerPresentationViewModel Model;
  Model.CapsuleRadius = 42.0f;
  Model.CapsuleHalfHeight = 96.0f;
  Model.FollowCameraArmLength = 400.0f;
  Model.RotationRateYaw = 500.0f;
  Model.JumpZVelocity = 500.0f;
  Model.AirControl = 0.35f;
  Model.MaxWalkSpeed = 500.0f;
  Model.MinAnalogWalkSpeed = 20.0f;
  Model.BrakingDecelerationWalking = 2000.0f;
  Model.BrakingDecelerationFalling = 1500.0f;
  Model.MeshRelativeLocation =
      FVector(0.0f, 0.0f, -Model.CapsuleHalfHeight);
  Model.MeshRelativeRotation = FRotator(0.0f, -90.0f, 0.0f);
  return Model;
}

inline FPlayerState ReducePlayerMovementInputObserved(
    const FPlayerState &State,
    const rtk::PayloadAction<FPlayerMovementInputRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FPlayerState Next) -> FPlayerState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.MovementInput = ReduceMovementInput(Action.PayloadValue);
            return Next;
          })
      .val;
}

inline FPlayerState ReducePlayerPresentationRequested(
    const FPlayerState &State,
    const rtk::PayloadAction<FPlayerPresentationRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FPlayerState Next) -> FPlayerState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.Presentation = ReducePlayerPresentation(Action.PayloadValue);
            return Next;
          })
      .val;
}

} // namespace PlayerReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
