#include "Features/Systems/Landmarks/SystemsLandmarksSlice.h"

#include "Features/Systems/Landmarks/SystemsLandmarksAdapters.h"

namespace ForbocAI {
namespace Game {
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
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"

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


#include "Features/Systems/Landmarks/SystemsLandmarksAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkFactories {

FLandmarkState CreateInitialState() {
  FLandmarkState State;
  State.Items = LandmarkAdapters::LandmarkAdapter().getInitialState();
  return State;
}

FLandmark Landmark(const FLandmarkSource &Source) {
  FLandmark Result;
  Result.Id = Source.Id;
  Result.Label = Source.Label;
  Result.Kind = Source.Kind;
  Result.Location = Source.Location;
  Result.Rotation = Source.Rotation;
  Result.Scale = Source.Scale;
  return Result;
}

} // namespace LandmarkFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
