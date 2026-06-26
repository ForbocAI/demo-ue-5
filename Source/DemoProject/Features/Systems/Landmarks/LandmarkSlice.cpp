#include "Features/Systems/Landmarks/LandmarkSlice.h"

#include "Features/Systems/Landmarks/LandmarkActions.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"
#include "Features/Systems/Landmarks/LandmarkReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace LandmarkSlice {

const rtk::Slice<FLandmarkState> &GetSlice() {
  static const rtk::Slice<FLandmarkState> Slice = []() {
    rtk::SliceBuilder<FLandmarkState> Builder =
        rtk::sliceBuilder<FLandmarkState>(
            TEXT("landmarks"), LandmarkFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, LandmarkActions::LandmarksSeeded(),
                                LandmarkReducers::ReduceLandmarksSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace LandmarkSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
