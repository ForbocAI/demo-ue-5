#include "Features/Systems/Townspeople/TownspersonSlice.h"

#include "Features/Systems/Townspeople/TownspersonActions.h"
#include "Features/Systems/Townspeople/TownspersonFactories.h"
#include "Features/Systems/Townspeople/TownspersonReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonSlice {

const rtk::Slice<FTownspersonState> &GetSlice() {
  static const rtk::Slice<FTownspersonState> Slice = []() {
    rtk::SliceBuilder<FTownspersonState> Builder =
        rtk::sliceBuilder<FTownspersonState>(
            TEXT("townspeople"), TownspersonFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder,
                                TownspersonActions::TownspeopleSeeded(),
                                TownspersonReducers::ReduceTownspeopleSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace TownspersonSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
