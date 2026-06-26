#include "Features/Systems/Nature/NatureSlice.h"

#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Systems/Nature/NatureFactories.h"
#include "Features/Systems/Nature/NatureReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureSlice {

const rtk::Slice<FNatureState> &GetSlice() {
  static const rtk::Slice<FNatureState> Slice = []() {
    rtk::SliceBuilder<FNatureState> Builder =
        rtk::sliceBuilder<FNatureState>(
            TEXT("nature"), NatureFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, NatureActions::NatureSeeded(),
                                NatureReducers::ReduceNatureSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace NatureSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
