#include "Features/Systems/SystemsActions.h"

#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Features/Systems/State/StateSlice.h"
#include "Store.h"
#include "Core/fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeActions {

/** User Story: As a features systems consumer, I need to invoke dispatch request player spawn through a stable signature so the features systems workflow remains explicit and composable. @fn func::AsyncResult<FPointPayload> DispatchRequestPlayerSpawn() */
func::AsyncResult<FPointPayload> DispatchRequestPlayerSpawn() {
  return Store::GetStore().dispatch(RuntimeSlice::RequestPlayerSpawn());
}

/** User Story: As a features systems consumer, I need to invoke dispatch request level view payload through a stable signature so the features systems workflow remains explicit and composable. @fn func::AsyncResult<FRuntimeLevelViewPayload> DispatchRequestLevelViewPayload() */
func::AsyncResult<FRuntimeLevelViewPayload> DispatchRequestLevelViewPayload() {
  return Store::GetStore().dispatch(RuntimeSlice::RequestLevelViewPayload());
}

/** User Story: As a features systems consumer, I need to invoke dispatch townsperson candidates observed through a stable signature so the features systems workflow remains explicit and composable. @fn void DispatchTownspersonCandidatesObserved( const FCandidatesObserved &Payload) */
void DispatchTownspersonCandidatesObserved(
    const FCandidatesObserved &Payload) {
  Store::GetStore().dispatch(
      InteractionSlice::TownspersonCandidatesObserved(Payload));
}

/** User Story: As a features systems consumer, I need to invoke dispatch townsperson interaction source observed through a stable signature so the features systems workflow remains explicit and composable. @fn void DispatchTownspersonInteractionSourceObserved( const FRuntimeTownspersonInteractionSource &Payload) */
void DispatchTownspersonInteractionSourceObserved(
    const FRuntimeTownspersonInteractionSource &Payload) {
  Store::GetStore().dispatch(TownspersonInteractionSourceObserved()(Payload));
}

/** User Story: As a features systems consumer, I need to invoke dispatch observe runtime stats tick through a stable signature so the features systems workflow remains explicit and composable. @fn void DispatchObserveRuntimeStatsTick(::UWorld *World, float DeltaSeconds) */
void DispatchObserveRuntimeStatsTick(::UWorld *World, float DeltaSeconds) {
  func::executeAsync(Store::GetStore().dispatch(
      RenderingThunks::ObserveRuntimeStatsTick(World, DeltaSeconds)));
}

} // namespace RuntimeActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI


#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"
#include "Store.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeDispatch {

/** User Story: As a features systems consumer, I need to invoke dispatch through a stable signature so the features systems workflow remains explicit and composable. @fn void Dispatch(const rtk::AnyAction &Action) */
void Dispatch(const rtk::AnyAction &Action) {
  Store::GetStore().dispatch(Action);
}

/** User Story: As a features systems consumer, I need to invoke dispatch through a stable signature so the features systems workflow remains explicit and composable. @fn void Dispatch(const rtk::ThunkAction<void, FRuntimeState> &Thunk) */
void Dispatch(const rtk::ThunkAction<void, FRuntimeState> &Thunk) {
  Store::GetStore().dispatch(Thunk);
}

/** User Story: As a features systems consumer, I need to invoke dispatch through a stable signature so the features systems workflow remains explicit and composable. @fn template <typename Result> func::AsyncResult<Result> Dispatch(const rtk::ThunkAction<Result, FRuntimeState> &Thunk) */
template <typename Result>
func::AsyncResult<Result>
Dispatch(const rtk::ThunkAction<Result, FRuntimeState> &Thunk) {
  return Store::GetStore().dispatch(Thunk);
}

/** User Story: As a features systems consumer, I need to invoke dispatch through a stable signature so the features systems workflow remains explicit and composable. @fn template func::AsyncResult<FPointPayload> Dispatch(const rtk::ThunkAction<FPointPayload, FRuntimeState> &Thunk) */
template func::AsyncResult<FPointPayload>
Dispatch(const rtk::ThunkAction<FPointPayload, FRuntimeState> &Thunk);

/** User Story: As a features systems consumer, I need to invoke dispatch through a stable signature so the features systems workflow remains explicit and composable. @fn template func::AsyncResult<FRuntimeLevelViewPayload> Dispatch(const rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState> &Thunk) */
template func::AsyncResult<FRuntimeLevelViewPayload>
Dispatch(const rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState> &Thunk);

} // namespace RuntimeDispatch
} // namespace Level
} // namespace Game
} // namespace ForbocAI
