#include "Features/Systems/Rendering/SystemsRenderingActions.h"

#include "Features/Systems/Rendering/RenderingThunks.h"
#include "Features/Systems/Rendering/Payload/Application/PayloadApplicationAdapters.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingActions {

/** User Story: As a features systems rendering consumer, I need to invoke rendering profile observed through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved() */
const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved() {
  static const func::Lazy<rtk::ActionCreator<FRenderingPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FRenderingPayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FRenderingPayload>(
            TEXT("systems/rendering/profileObserved"));
      });
  return func::eval(Creator);
}

/** User Story: As a features systems rendering consumer, I need to invoke runtime stats sampled through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const rtk::ActionCreator<FRuntimeStatsSamplePayload> &RuntimeStatsSampled() */
const rtk::ActionCreator<FRuntimeStatsSamplePayload> &RuntimeStatsSampled() {
  static const func::Lazy<rtk::ActionCreator<FRuntimeStatsSamplePayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FRuntimeStatsSamplePayload> {
        return rtk::createAction<FRuntimeStatsSamplePayload>(
            TEXT("systems/rendering/runtimeStatsSampled"));
      });
  return func::eval(Creator);
}

/** User Story: As a features systems rendering consumer, I need to invoke apply runtime profile through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) */
void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) {
  RenderingThunks::ApplyRuntimeProfile(Request);
}

/** User Story: As a features systems rendering consumer, I need to invoke load blockout material through a stable signature so the features systems rendering workflow remains explicit and composable. @fn UMaterialInterface *LoadBlockoutMaterial(const FString &Path) */
UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return RenderingThunks::LoadBlockoutMaterial(Path);
}

/** User Story: As a features systems rendering consumer, I need to invoke apply texture through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyTexture(const FLevelRetroTextureApply &Request) */
void ApplyTexture(const FLevelRetroTextureApply &Request) {
  RenderingThunks::ApplyTexture(Request);
}

} // namespace RenderingActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
