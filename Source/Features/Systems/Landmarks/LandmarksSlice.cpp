#include "Features/Systems/Landmarks/LandmarksSlice.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Systems/Landmarks/SystemsLandmarksAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkReducers {

/** User Story: As a features systems landmarks consumer, I need to invoke reduce landmarks seeded through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn FLandmarkState ReduceLandmarksSeeded(const FLandmarkState &State, const rtk::PayloadAction<TArray<FLandmark>> &Action) */
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

#include "Features/Systems/Landmarks/LandmarksActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LandmarkSlice {

/** User Story: As a features systems landmarks consumer, I need to invoke get slice through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn const rtk::Slice<FLandmarkState> &GetSlice() */
const rtk::Slice<FLandmarkState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FLandmarkState>> Slice =
      func::lazy([]() -> rtk::Slice<FLandmarkState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FLandmarkState>(
          TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV5CC0D75D70F0), LandmarkFactories::CreateInitialState(),
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

/** User Story: As a features systems landmarks consumer, I need to invoke create initial state through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn FLandmarkState CreateInitialState() */
FLandmarkState CreateInitialState() {
  FLandmarkState State;
  State.Items = LandmarkAdapters::LandmarkAdapter().getInitialState();
  return State;
}

} // namespace LandmarkFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
