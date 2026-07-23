#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/SystemsInteractionTypes.h"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeActions {

/** User Story: As a features systems consumer, I need to invoke dispatch request player spawn through a stable signature so the features systems workflow remains explicit and composable. @fn func::AsyncResult<FPointPayload> DispatchRequestPlayerSpawn() */
func::AsyncResult<FPointPayload> DispatchRequestPlayerSpawn();
/** User Story: As a features systems consumer, I need to invoke dispatch request level view payload through a stable signature so the features systems workflow remains explicit and composable. @fn func::AsyncResult<FRuntimeLevelViewPayload> DispatchRequestLevelViewPayload() */
func::AsyncResult<FRuntimeLevelViewPayload> DispatchRequestLevelViewPayload();
/** User Story: As a features systems consumer, I need to invoke dispatch townsperson candidates observed through a stable signature so the features systems workflow remains explicit and composable. @fn void DispatchTownspersonCandidatesObserved( const FCandidatesObserved &Payload) */
void DispatchTownspersonCandidatesObserved(
    const FCandidatesObserved &Payload);
/** User Story: As a features systems consumer, I need to invoke dispatch townsperson interaction source observed through a stable signature so the features systems workflow remains explicit and composable. @fn void DispatchTownspersonInteractionSourceObserved( const FRuntimeTownspersonInteractionSource &Payload) */
void DispatchTownspersonInteractionSourceObserved(
    const FRuntimeTownspersonInteractionSource &Payload);
/** User Story: As a features systems consumer, I need to invoke dispatch observe runtime stats tick through a stable signature so the features systems workflow remains explicit and composable. @fn void DispatchObserveRuntimeStatsTick(::UWorld *World, float DeltaSeconds) */
void DispatchObserveRuntimeStatsTick(::UWorld *World, float DeltaSeconds);

/**
 * @fn inline const rtk::ActionCreator<FRuntimeState> &RuntimeHydrated()
 * @brief RTK action creator for replacing the runtime root state.
 * User Story: As a features systems consumer, I need to invoke runtime hydrated through a stable signature so the features systems workflow remains explicit and composable.
 */
inline const rtk::ActionCreator<FRuntimeState> &RuntimeHydrated() {
  static const func::Lazy<rtk::ActionCreator<FRuntimeState>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FRuntimeState> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FRuntimeState>(TEXT("runtime/hydrated"));
      });
  return func::eval(Creator);
}

/**
 * @fn inline const rtk::ActionCreator<FRuntimeTownspersonInteractionSource> & TownspersonInteractionSourceObserved()
 * @brief RTK action creator for observed townsperson dialogue source data.
 * User Story: As a features systems consumer, I need to invoke townsperson interaction source observed through a stable signature so the features systems workflow remains explicit and composable.
 */
inline const rtk::ActionCreator<FRuntimeTownspersonInteractionSource> &
TownspersonInteractionSourceObserved() {
  static const func::Lazy<
      rtk::ActionCreator<FRuntimeTownspersonInteractionSource>>
      Creator =
          func::lazy([]()
                         -> rtk::ActionCreator<
                             FRuntimeTownspersonInteractionSource> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FRuntimeTownspersonInteractionSource>(
                TEXT("runtime/townspersonInteractionSourceObserved"));
          });
  return func::eval(Creator);
}

} // namespace RuntimeActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/State/SystemsStateTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeDispatch {

/** User Story: As a features systems consumer, I need to invoke dispatch through a stable signature so the features systems workflow remains explicit and composable. @fn void Dispatch(const rtk::AnyAction &Action) */
void Dispatch(const rtk::AnyAction &Action);
/** User Story: As a features systems consumer, I need to invoke dispatch through a stable signature so the features systems workflow remains explicit and composable. @fn void Dispatch(const rtk::ThunkAction<void, FRuntimeState> &Thunk) */
void Dispatch(const rtk::ThunkAction<void, FRuntimeState> &Thunk);

/** User Story: As a features systems consumer, I need to invoke dispatch through a stable signature so the features systems workflow remains explicit and composable. @fn template <typename Result> func::AsyncResult<Result> Dispatch(const rtk::ThunkAction<Result, FRuntimeState> &Thunk) */
template <typename Result>
func::AsyncResult<Result>
Dispatch(const rtk::ThunkAction<Result, FRuntimeState> &Thunk);

} // namespace RuntimeDispatch
} // namespace Level
} // namespace Game
} // namespace ForbocAI
