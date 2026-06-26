#include "Features/Systems/Landmarks/LandmarkAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkAdapters {

const rtk::EntityAdapterOps<FLandmark> &LandmarkAdapter() {
  static const rtk::EntityAdapterOps<FLandmark> Adapter =
      rtk::createEntityAdapter<FLandmark>(
          [](const FLandmark &Landmark) -> FString { return Landmark.Id; });
  return Adapter;
}

} // namespace LandmarkAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
