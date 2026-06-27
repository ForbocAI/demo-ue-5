#include "Features/Systems/Rendering/RenderingReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingReducers {

FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input) {
  return (func::pipe(Input) |
          [](FLevelRetroTextureApply Value) -> FLevelRetroTextureApply {
    return Value;
  })
      .val;
}

FRenderingState ReduceRenderingProfileObserved(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.RuntimeProfile = Action.PayloadValue.RuntimeProfile;
            Next.TextureCatalog = Action.PayloadValue.TextureCatalog;
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace RenderingReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
