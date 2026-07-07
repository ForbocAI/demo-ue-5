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

void DispatchObserveRuntimeStatsTick(::UWorld *World, float DeltaSeconds) {
  func::executeAsync(Store::GetStore().dispatch(
      RenderingThunks::ObserveRuntimeStatsTick(World, DeltaSeconds)));
}

} // namespace RuntimeActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
