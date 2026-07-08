#include "Features/Systems/Rendering/SystemsRenderingActions.h"

#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Features/Systems/Rendering/Payload/Application/PayloadApplicationAdapters.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingActions {

const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved() {
  static const func::Lazy<rtk::ActionCreator<FRenderingPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FRenderingPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FRenderingPayload>(
            TEXT("systems/rendering/profileObserved"));
      });
  return func::eval(Creator);
}

const rtk::ActionCreator<FRuntimeStatsSamplePayload> &RuntimeStatsSampled() {
  static const func::Lazy<rtk::ActionCreator<FRuntimeStatsSamplePayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FRuntimeStatsSamplePayload> {
        return rtk::createAction<FRuntimeStatsSamplePayload>(
            TEXT("systems/rendering/runtimeStatsSampled"));
      });
  return func::eval(Creator);
}

void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) {
  RenderingThunks::ApplyRuntimeProfile(Request);
}

UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return RenderingThunks::LoadBlockoutMaterial(Path);
}

void ApplyTexture(const FLevelRetroTextureApply &Request) {
  RenderingThunks::ApplyTexture(Request);
}

} // namespace RenderingActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
