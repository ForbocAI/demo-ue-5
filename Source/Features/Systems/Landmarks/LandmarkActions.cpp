#include "Features/Systems/Landmarks/LandmarkActions.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarkActions {

const rtk::ActionCreator<TArray<FLandmark>> &LandmarksSeeded() {
  static const func::Lazy<rtk::ActionCreator<TArray<FLandmark>>> Creator =
      func::lazy([]() -> rtk::ActionCreator<TArray<FLandmark>> {
        return rtk::createAction<TArray<FLandmark>>(TEXT("landmarks/seeded"));
      });
  return func::eval(Creator);
}

} // namespace LandmarkActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
