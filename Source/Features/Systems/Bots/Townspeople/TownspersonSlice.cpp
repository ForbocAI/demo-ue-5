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
        return rtk::createSlice<FTownspersonState>(
          TEXT("townspeople"), TownspersonFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FTownspersonState> &Builder) {
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                TownspersonReducers::ReduceTownspeopleSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace TownspersonSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
