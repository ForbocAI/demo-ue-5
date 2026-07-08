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

func::AsyncResult<FPointPayload> DispatchRequestPlayerSpawn();
func::AsyncResult<FRuntimeLevelViewPayload> DispatchRequestLevelViewPayload();
void DispatchTownspersonCandidatesObserved(
    const FCandidatesObserved &Payload);
void DispatchTownspersonInteractionSourceObserved(
    const FRuntimeTownspersonInteractionSource &Payload);
void DispatchObserveRuntimeStatsTick(::UWorld *World, float DeltaSeconds);

/**
 * @brief RTK action creator for replacing the runtime root state.
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
 * @brief RTK action creator for observed townsperson dialogue source data.
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

void Dispatch(const rtk::AnyAction &Action);
void Dispatch(const rtk::ThunkAction<void, FRuntimeState> &Thunk);

template <typename Result>
func::AsyncResult<Result>
Dispatch(const rtk::ThunkAction<Result, FRuntimeState> &Thunk);

} // namespace RuntimeDispatch
} // namespace Level
} // namespace Game
} // namespace ForbocAI
