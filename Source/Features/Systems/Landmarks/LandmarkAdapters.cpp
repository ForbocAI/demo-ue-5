#include "Features/Systems/Landmarks/LandmarkAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkAdapters {

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
