#include "Features/Systems/Landmarks/LandmarkSlice.h"

#include "Features/Systems/Landmarks/LandmarkActions.h"
#include "Features/Systems/Landmarks/LandmarkFactories.h"
#include "Features/Systems/Landmarks/LandmarkReducers.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkSlice {

const rtk::Slice<FLandmarkState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FLandmarkState>> Slice =
      func::lazy([]() -> rtk::Slice<FLandmarkState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FLandmarkState>(
          TEXT("landmarks"), LandmarkFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FLandmarkState> &Builder) {
    Builder.addCase(LandmarkActions::LandmarksSeeded(),
                                LandmarkReducers::ReduceLandmarksSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace LandmarkSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
