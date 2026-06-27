#include "Features/Systems/Rendering/RenderingActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingActions {

const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved() {
  static const func::Lazy<rtk::ActionCreator<FRenderingPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FRenderingPayload> {
        return rtk::createAction<FRenderingPayload>(
            TEXT("systems/rendering/profileObserved"));
      });
  return func::eval(Creator);
}

} // namespace RenderingActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
