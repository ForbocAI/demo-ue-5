#include "Features/Components/Data/DataSlice.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataReducers {

/** User Story: As a features components data consumer, I need to invoke reduce data loaded through a stable signature so the features components data workflow remains explicit and composable. @fn FDataState ReduceDataLoaded(const FDataState &State, const rtk::PayloadAction<FDataLoadedPayload> &Action) */
FDataState
ReduceDataLoaded(const FDataState &State,
                 const rtk::PayloadAction<FDataLoadedPayload> &Action) {
  FDataState Next = State;
  Next.Path = func::just(Action.PayloadValue.RelativePath);
  Next.bLoadSucceeded = Action.PayloadValue.bLoaded;
  Next.LoadedObjectCount =
      Action.PayloadValue.bLoaded ? State.LoadedObjectCount + FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4
                                  : State.LoadedObjectCount;
  return Next;
}

} // namespace DataReducers
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataSlice {

namespace {

/** User Story: As a features components data consumer, I need to invoke create initial state through a stable signature so the features components data workflow remains explicit and composable. @fn FDataState CreateInitialState() */
FDataState CreateInitialState() { return FDataState{}; }

} // namespace

/** User Story: As a features components data consumer, I need to invoke get slice through a stable signature so the features components data workflow remains explicit and composable. @fn const rtk::Slice<FDataState> &GetSlice() */
const rtk::Slice<FDataState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FDataState>> Slice =
      func::lazy([]() -> rtk::Slice<FDataState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FDataState>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV13EF85B46077), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FDataState> &Builder) {
              Builder.addCase(DataActions::DataLoaded(),
                              DataReducers::ReduceDataLoaded);
            });
      });
  return func::eval(Slice);
}

} // namespace DataSlice
} // namespace Data
} // namespace Game
} // namespace ForbocAI
