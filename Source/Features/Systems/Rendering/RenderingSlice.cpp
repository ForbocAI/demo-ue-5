#include "Features/Systems/Rendering/RenderingSlice.h"

#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Rendering/RenderingSelectors.h"
#include "Features/Systems/Rendering/RenderingThunks.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingSlice {

FRenderingState CreateInitialState() {
  return (func::pipe(FRenderingState{}) |
          [](FRenderingState State) -> FRenderingState {
            State.LastActionId = func::nothing<FString>();
            State.RuntimeProfile = RenderingSelectors::SelectRuntimeProfile();
            State.TextureCatalog = RenderingSelectors::SelectTextureCatalog();
            State.bReady = false;
            return State;
          })
      .val;
}

const rtk::Slice<FRenderingState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FRenderingState>> Slice =
      func::lazy([]() -> rtk::Slice<FRenderingState> {
        return rtk::createSlice<FRenderingState>(
            TEXT("systems/rendering"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FRenderingState> &Builder) {
              Builder.addCase(
                  RenderingActions::RenderingProfileObserved(),
                  RenderingReducers::ReduceRenderingProfileObserved);
            });
      });
  return func::eval(Slice);
}

void ApplyRuntimeProfile() { RenderingThunks::ApplyRuntimeProfile(); }

UMaterialInterface *LoadBlockoutMaterial() {
  return RenderingThunks::LoadBlockoutMaterial();
}

void ApplyTexture(const FLevelRetroTextureApply &Request) {
  RenderingThunks::ApplyTexture(Request);
}

} // namespace RenderingSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
