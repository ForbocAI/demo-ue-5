#include "Features/Systems/Bots/Horses/Slice.h"

#include "Features/Systems/Bots/Horses/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseReducers {

FHorseState ReduceHorsesSeeded(
    const FHorseState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FHorseState Next) -> FHorseState {
  Next.Items =
      HorseAdapters::HorseAdapter().setAll(State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

} // namespace HorseReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Horses/Actions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSlice {

const rtk::Slice<FHorseState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FHorseState>> Slice =
      func::lazy([]() -> rtk::Slice<FHorseState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
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
} // namespace Game
} // namespace ForbocAI


#include "Features/Systems/Bots/Horses/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseFactories {

FHorseState CreateInitialState() {
  FHorseState State;
  State.Items = HorseAdapters::HorseAdapter().getInitialState();
  return State;
}

} // namespace HorseFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
