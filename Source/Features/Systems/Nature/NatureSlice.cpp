#include "Features/Systems/Nature/NatureSlice.h"

#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Systems/Nature/NatureFactories.h"
#include "Features/Systems/Nature/NatureReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace NatureSlice {

const rtk::Slice<FNatureState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FNatureState>> Slice =
      func::lazy([]() -> rtk::Slice<FNatureState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FNatureState>(
      TEXT("nature"), NatureFactories::CreateInitialState(),
      [](rtk::ActionReducerMapBuilder<FNatureState> &Builder) {
    Builder.addCase(NatureActions::NatureSeeded(),
                                NatureReducers::ReduceNatureSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace NatureSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
