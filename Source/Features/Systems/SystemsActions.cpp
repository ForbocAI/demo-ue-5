#include "Features/Systems/SystemsActions.h"

#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Features/Systems/SystemsSlice.h"
#include "Store.h"
#include "Core/ue_fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeActions {

func::AsyncResult<FPointPayload> DispatchRequestPlayerSpawn() {
  return Store::GetStore().dispatch(RuntimeSlice::RequestPlayerSpawn());
}

func::AsyncResult<FRuntimeLevelViewPayload> DispatchRequestLevelViewPayload() {
  return Store::GetStore().dispatch(RuntimeSlice::RequestLevelViewPayload());
}

void DispatchTownspersonCandidatesObserved(
    const FCandidatesObserved &Payload) {
  Store::GetStore().dispatch(
      InteractionSlice::TownspersonCandidatesObserved(Payload));
}

void DispatchTownspersonInteractionSourceObserved(
    const FRuntimeTownspersonInteractionSource &Payload) {
  Store::GetStore().dispatch(TownspersonInteractionSourceObserved()(Payload));
}

void DispatchObserveRuntimeStatsTick(::UWorld *World, float DeltaSeconds) {
  func::executeAsync(Store::GetStore().dispatch(
      RenderingThunks::ObserveRuntimeStatsTick(World, DeltaSeconds)));
}

} // namespace RuntimeActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI


#include "Features/Systems/Level/View/ViewTypes.h"
#include "Features/Systems/View/Spawn/ViewSpawnTypes.h"
#include "Store.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeDispatch {

void Dispatch(const rtk::AnyAction &Action) {
  Store::GetStore().dispatch(Action);
}

void Dispatch(const rtk::ThunkAction<void, FRuntimeState> &Thunk) {
  Store::GetStore().dispatch(Thunk);
}

template <typename Result>
func::AsyncResult<Result>
Dispatch(const rtk::ThunkAction<Result, FRuntimeState> &Thunk) {
  return Store::GetStore().dispatch(Thunk);
}

template func::AsyncResult<FPointPayload>
Dispatch(const rtk::ThunkAction<FPointPayload, FRuntimeState> &Thunk);

template func::AsyncResult<FRuntimeLevelViewPayload>
Dispatch(const rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState> &Thunk);

} // namespace RuntimeDispatch
} // namespace Level
} // namespace Game
} // namespace ForbocAI
