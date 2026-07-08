#include "Features/Components/Data/Slice.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace DataReducers {

FDataState
ReduceDataLoaded(const FDataState &State,
                 const rtk::PayloadAction<FDataLoadedPayload> &Action) {
  FDataState Next = State;
  Next.Path = func::just(Action.PayloadValue.RelativePath);
  Next.bLoadSucceeded = Action.PayloadValue.bLoaded;
  Next.LoadedObjectCount =
      Action.PayloadValue.bLoaded ? State.LoadedObjectCount + 1
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

FDataState CreateInitialState() { return FDataState{}; }

} // namespace

const rtk::Slice<FDataState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FDataState>> Slice =
      func::lazy([]() -> rtk::Slice<FDataState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FDataState>(
            TEXT("data"), CreateInitialState(),
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
