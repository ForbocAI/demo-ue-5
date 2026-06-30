#include "Features/Systems/Rendering/RenderingActions.h"

namespace ForbocAI {
namespace Demo {
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

const rtk::ActionCreator<FRenderingPresentationRequest> &
TownspersonPresentationRequested() {
  static const func::Lazy<rtk::ActionCreator<FRenderingPresentationRequest>>
      Creator =
          func::lazy(
              []() -> rtk::ActionCreator<FRenderingPresentationRequest> {
                // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
                return rtk::createAction<FRenderingPresentationRequest>(
                    TEXT("systems/rendering/townspersonPresentationRequested"));
              });
  return func::eval(Creator);
}

const rtk::ActionCreator<FRenderingPresentationRequest> &
HorsePresentationRequested() {
  static const func::Lazy<rtk::ActionCreator<FRenderingPresentationRequest>>
      Creator =
          func::lazy(
              []() -> rtk::ActionCreator<FRenderingPresentationRequest> {
                // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
                return rtk::createAction<FRenderingPresentationRequest>(
                    TEXT("systems/rendering/horsePresentationRequested"));
              });
  return func::eval(Creator);
}

} // namespace RenderingActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
