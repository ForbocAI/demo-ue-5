#include "Features/Systems/Horses/HorseSlice.h"

#include "Features/Systems/Horses/HorseActions.h"
#include "Features/Systems/Horses/HorseFactories.h"
#include "Features/Systems/Horses/HorseReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseSlice {

const rtk::Slice<FHorseState> &GetSlice() {
  static const rtk::Slice<FHorseState> Slice = []() {
    rtk::SliceBuilder<FHorseState> Builder =
        rtk::sliceBuilder<FHorseState>(
            TEXT("horses"), HorseFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, HorseActions::HorsesSeeded(),
                                HorseReducers::ReduceHorsesSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace HorseSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
