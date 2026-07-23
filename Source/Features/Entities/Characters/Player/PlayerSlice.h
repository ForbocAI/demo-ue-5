#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Entities/Characters/Player/CharactersPlayerTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace PlayerReducers {

/** User Story: As a entities characters player consumer, I need to invoke reduce player observed through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline FPlayerState ReducePlayerObserved(const FPlayerState &State, const rtk::PayloadAction<FPlayerPayload> &Action) */
inline FPlayerState
ReducePlayerObserved(const FPlayerState &State,
                     const rtk::PayloadAction<FPlayerPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FPlayerState Next) -> FPlayerState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

/** User Story: As a entities characters player consumer, I need to invoke reduce yaw rotation through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline FRotator ReduceYawRotation(const FMovementInputRequest &Request) */
inline FRotator
ReduceYawRotation(const FMovementInputRequest &Request) {
  return FRotator(0.0f, Request.ControlRotation.Yaw, 0.0f);
}

/** User Story: As a entities characters player consumer, I need to invoke reduce movement input through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline FMovementInputViewModel ReduceMovementInput( const FMovementInputRequest &Request) */
inline FMovementInputViewModel ReduceMovementInput(
    const FMovementInputRequest &Request) {
  const FRotator YawRotation = ReduceYawRotation(Request);
  FMovementInputViewModel Model;
  Model.ForwardDirection =
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  Model.RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
  Model.ForwardScale = Request.Forward;
  Model.RightScale = Request.Right;
  Model.bShouldMove = Request.bControllerAvailable;
  return Model;
}

/**
 * @fn inline FPresentationViewModel ReducePlayerPresentation( const ForbocAI::Game::Data::FPresentationSettings &Settings)
 * @brief Maps JSON-backed player presentation settings into selector state.
 *
 *
 * User story: As a player, camera, movement, mesh, and input assets can be
 * tuned from data while the Redux reducer remains the owner of presentation
 * state.
 * User Story: As a entities characters player consumer, I need to invoke reduce player presentation through a stable signature so the entities characters player workflow remains explicit and composable.
 */
inline FPresentationViewModel ReducePlayerPresentation(
    const ForbocAI::Game::Data::FPresentationSettings &Settings) {
  return {Settings.Capsule,      Settings.Camera,
          Settings.Movement,     Settings.Braking,
          Settings.MeshTransform, Settings.MeshLod,
          Settings.MeshBehavior, Settings.MeshAssets,
          Settings.InputActions, Settings.InputMappings};
}

/** User Story: As a entities characters player consumer, I need to invoke reduce player presentation requested through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline FPlayerState ReducePlayerPresentationRequested( const FPlayerState &State, const rtk::PayloadAction<FPlayerPresentationRequest> &Action) */
inline FPlayerState ReducePlayerPresentationRequested(
    const FPlayerState &State,
    const rtk::PayloadAction<FPlayerPresentationRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FPlayerState Next) -> FPlayerState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            return Next;
          })
      .val;
}

} // namespace PlayerReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Entities/Characters/Player/PlayerSelectors.h"
#include "Features/Entities/Characters/Player/CharactersPlayerTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace PlayerSlice {

/** User Story: As a entities characters player consumer, I need to invoke create initial state through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline FPlayerState CreateInitialState() */
inline FPlayerState CreateInitialState() {
  return (func::pipe(FPlayerState{}) |
          [](FPlayerState State) -> FPlayerState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

/** User Story: As a entities characters player consumer, I need to invoke get slice through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline const rtk::Slice<FPlayerState> &GetSlice() */
inline const rtk::Slice<FPlayerState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FPlayerState>> Slice =
      func::lazy([]() -> rtk::Slice<FPlayerState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FPlayerState>(
            TEXT("entities/player"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FPlayerState> &Builder) {
              Builder.addCase(PlayerActions::PlayerObserved(),
                              PlayerReducers::ReducePlayerObserved);
              Builder.addCase(
                  PlayerActions::PlayerPresentationRequested(),
                  PlayerReducers::ReducePlayerPresentationRequested);
            });
      });
  return func::eval(Slice);
}

} // namespace PlayerSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
