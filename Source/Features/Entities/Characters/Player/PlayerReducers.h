#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Entities/Characters/Player/PlayerTypes.h"

namespace ForbocAI {
namespace Game {
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

/**
 * @brief Maps JSON-backed player presentation settings into selector state.
 *
 * @signature FPlayerPresentationViewModel ReducePlayerPresentation(const
 * ForbocAI::Game::Data::FPlayerPresentationSettings &Settings)
 *
 * User story: As a player, camera, movement, mesh, and input assets can be
 * tuned from data while the Redux reducer remains the owner of presentation
 * state.
 */
inline FPlayerPresentationViewModel ReducePlayerPresentation(
    const ForbocAI::Game::Data::FPlayerPresentationSettings &Settings) {
  FPlayerPresentationViewModel Model;
  Model.CapsuleRadius = Settings.CapsuleRadius;
  Model.CapsuleHalfHeight = Settings.CapsuleHalfHeight;
  Model.FollowCameraArmLength = Settings.FollowCameraArmLength;
  Model.RotationRateYaw = Settings.RotationRateYaw;
  Model.JumpZVelocity = Settings.JumpZVelocity;
  Model.AirControl = Settings.AirControl;
  Model.MaxWalkSpeed = Settings.MaxWalkSpeed;
  Model.MinAnalogWalkSpeed = Settings.MinAnalogWalkSpeed;
  Model.BrakingDecelerationWalking = Settings.BrakingDecelerationWalking;
  Model.BrakingDecelerationFalling = Settings.BrakingDecelerationFalling;
  Model.MeshRelativeLocation = Settings.MeshRelativeLocation;
  Model.MeshRelativeRotation = Settings.MeshRelativeRotation;
  Model.SkeletalMeshForcedLodModel = Settings.SkeletalMeshForcedLodModel;
  Model.SkeletalMeshMinLodModel = Settings.SkeletalMeshMinLodModel;
  Model.MeshCullDistance = Settings.MeshCullDistance;
  Model.bMeshCastShadow = Settings.bMeshCastShadow;
  Model.bMeshComponentTickEnabled = Settings.bMeshComponentTickEnabled;
  Model.bMeshUpdateRateOptimizationsEnabled =
      Settings.bMeshUpdateRateOptimizationsEnabled;
  Model.MeshPath = Settings.MeshPath;
  Model.AnimationBlueprintClassPath = Settings.AnimationBlueprintClassPath;
  Model.MoveActionPath = Settings.MoveActionPath;
  Model.LookActionPath = Settings.LookActionPath;
  Model.MouseLookActionPath = Settings.MouseLookActionPath;
  Model.JumpActionPath = Settings.JumpActionPath;
  Model.DefaultMappingContextPath = Settings.DefaultMappingContextPath;
  Model.MouseMappingContextPath = Settings.MouseMappingContextPath;
  return Model;
}

inline FPlayerState ReducePlayerPresentationRequested(
    const FPlayerState &State,
    const rtk::PayloadAction<FPlayerPresentationRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FPlayerState Next) -> FPlayerState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            return Next;
          })
      .val;
}

} // namespace PlayerReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
