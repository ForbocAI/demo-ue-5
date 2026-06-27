#include "Features/Systems/Landmarks/LandmarkReducers.h"

#include "Features/Systems/Landmarks/LandmarkAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LandmarkReducers {

FLandmarkState
ReduceLandmarksSeeded(const FLandmarkState &State,
                      const rtk::PayloadAction<TArray<FLandmark>> &Action) {
  return (func::pipe(State) | [&](FLandmarkState Next) -> FLandmarkState {
  Next.Items =
      LandmarkAdapters::LandmarkAdapter().setAll(State.Items,
                                                 Action.PayloadValue);
  return Next;
  }).val;
}

} // namespace LandmarkReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
