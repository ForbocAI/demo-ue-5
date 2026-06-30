#include "Features/Systems/Bots/Townspeople/TownspersonSlice.h"

#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonFactories.h"
#include "Features/Systems/Bots/Townspeople/TownspersonReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonSlice {

const rtk::Slice<FTownspersonState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FTownspersonState>> Slice =
      func::lazy([]() -> rtk::Slice<FTownspersonState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FTownspersonState>(
          TEXT("townspeople"), TownspersonFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FTownspersonState> &Builder) {
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                TownspersonReducers::ReduceTownspeopleSeeded);
    Builder.addCase(
        TownspersonActions::ViewDefaultsRequested(),
        TownspersonReducers::ReduceViewDefaultsRequested);
    Builder.addCase(
        TownspersonActions::InteractionOverlapObserved(),
        TownspersonReducers::ReduceInteractionOverlapObserved);
  });
      });
  return func::eval(Slice);
}

} // namespace TownspersonSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
