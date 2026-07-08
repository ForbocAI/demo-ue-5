#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkActions {

const rtk::ActionCreator<TArray<FLandmark>> &LandmarksSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FLandmark>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FLandmark>> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<TArray<FLandmark>>(TEXT("landmarks/seeded"));
      });
  return func::eval(Creator);
}

} // namespace LandmarkActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
