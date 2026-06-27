#include "Features/Systems/Bots/Horses/HorseSlice.h"

#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Horses/HorseFactories.h"
#include "Features/Systems/Bots/Horses/HorseReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace HorseSlice {

const rtk::Slice<FHorseState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FHorseState>> Slice =
      func::lazy([]() -> rtk::Slice<FHorseState> {
        return rtk::createSlice<FHorseState>(
      TEXT("horses"), HorseFactories::CreateInitialState(),
      [](rtk::ActionReducerMapBuilder<FHorseState> &Builder) {
    Builder.addCase(HorseActions::HorsesSeeded(),
                                HorseReducers::ReduceHorsesSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace HorseSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
