#include "Features/Systems/Landmarks/LandmarkActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkActions {

const rtk::ActionCreator<TArray<FLandmark>> &LandmarksSeeded() {
  static const rtk::ActionCreator<TArray<FLandmark>> Creator =
      rtk::createAction<TArray<FLandmark>>(TEXT("landmarks/seeded"));
  return Creator;
}

} // namespace LandmarkActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
