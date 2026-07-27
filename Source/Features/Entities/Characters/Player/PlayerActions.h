#pragma once
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/CharactersPlayerTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace PlayerActions {

/** User Story: As a entities characters player consumer, I need to invoke player observed through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FPlayerPayload> &PlayerObserved() */
inline const rtk::ActionCreator<FPlayerPayload> &PlayerObserved() {
  static const func::Lazy<rtk::ActionCreator<FPlayerPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FPlayerPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FPlayerPayload>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV211660182687));
      });
  return func::eval(Creator);
}

/** User Story: As a entities characters player consumer, I need to invoke player movement input observed through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FMovementInputRequest> & PlayerMovementInputObserved() */
inline const rtk::ActionCreator<FMovementInputRequest> &
PlayerMovementInputObserved() {
  static const func::Lazy<rtk::ActionCreator<FMovementInputRequest>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FMovementInputRequest> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FMovementInputRequest>(
                TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV31AA129D9530));
          });
  return func::eval(Creator);
}

/** User Story: As a entities characters player consumer, I need to invoke player presentation requested through a stable signature so the entities characters player workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FPlayerPresentationRequest> & PlayerPresentationRequested() */
inline const rtk::ActionCreator<FPlayerPresentationRequest> &
PlayerPresentationRequested() {
  static const func::Lazy<rtk::ActionCreator<FPlayerPresentationRequest>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FPlayerPresentationRequest> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FPlayerPresentationRequest>(
                TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV34A968CBE82E));
          });
  return func::eval(Creator);
}

} // namespace PlayerActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
