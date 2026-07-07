#include "Features/Systems/Runtime/RuntimeActions.h"

#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Store.h"
#include "Core/ue_fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeActions {

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

void DispatchObserveRuntimeStatsTick(::UWorld *World, float DeltaSeconds) {
  func::executeAsync(Store::GetStore().dispatch(
      RenderingThunks::ObserveRuntimeStatsTick(World, DeltaSeconds)));
}

template func::AsyncResult<FSpawnPointPayload>
Dispatch(const rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> &Thunk);

template func::AsyncResult<FRuntimeLevelViewPayload>
Dispatch(const rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState> &Thunk);

} // namespace RuntimeActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
