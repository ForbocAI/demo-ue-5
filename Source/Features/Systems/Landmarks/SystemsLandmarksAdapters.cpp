#include "Features/Systems/Landmarks/SystemsLandmarksAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkAdapters {

/** User Story: As a features systems landmarks consumer, I need to invoke landmark adapter through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn const rtk::EntityAdapter<FLandmark> &LandmarkAdapter() */
const rtk::EntityAdapter<FLandmark> &LandmarkAdapter() {
  static const func::Lazy<rtk::EntityAdapter<FLandmark>> Adapter =
      func::lazy([]() -> rtk::EntityAdapter<FLandmark> {
        return rtk::createEntityAdapter<FLandmark>(
          [](const FLandmark &Landmark) -> FString { return Landmark.Id; });
      });
  return func::eval(Adapter);
}

} // namespace LandmarkAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
