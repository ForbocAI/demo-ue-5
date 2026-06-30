#include "Features/Components/Data/DataSlice.h"

namespace ForbocAI {
namespace Demo {
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
} // namespace Demo
} // namespace ForbocAI
