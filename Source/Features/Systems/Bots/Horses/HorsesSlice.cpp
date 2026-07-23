#include "Features/Systems/Bots/Horses/HorsesSlice.h"

#include "Features/Systems/Bots/Horses/HorsesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseReducers {

/** User Story: As a systems bots horses consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots horses workflow remains explicit and composable. @fn FHorseState ReduceHorsesSeeded( const FHorseState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
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

#include "Features/Systems/Bots/Horses/HorsesActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSlice {

/** User Story: As a systems bots horses consumer, I need to invoke get slice through a stable signature so the systems bots horses workflow remains explicit and composable. @fn const rtk::Slice<FHorseState> &GetSlice() */
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


#include "Features/Systems/Bots/Horses/HorsesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseFactories {

/** User Story: As a systems bots horses consumer, I need to invoke create initial state through a stable signature so the systems bots horses workflow remains explicit and composable. @fn FHorseState CreateInitialState() */
FHorseState CreateInitialState() {
  FHorseState State;
  State.Items = HorseAdapters::HorseAdapter().getInitialState();
  return State;
}

} // namespace HorseFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
