#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkActions {

/** User Story: As a features systems landmarks consumer, I need to invoke landmarks seeded through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FLandmark>> &LandmarksSeeded() */
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
