#include "Features/Systems/Landmarks/LandmarkReducers.h"

#include "Features/Systems/Landmarks/LandmarkAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkReducers {

FLandmarkState
ReduceLandmarksSeeded(const FLandmarkState &State,
                      const rtk::Action<TArray<FLandmark>> &Action) {
  FLandmarkState Next = State;
  Next.Items =
      LandmarkAdapters::LandmarkAdapter().setAll(State.Items,
                                                 Action.PayloadValue);
  return Next;
}

} // namespace LandmarkReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
