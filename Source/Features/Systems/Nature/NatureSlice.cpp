#include "Features/Systems/Nature/SystemsNatureSlice.h"

#include "Features/Systems/Nature/Entity/EntityAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureReducers {

/** User Story: As a features systems nature consumer, I need to invoke reduce nature seeded through a stable signature so the features systems nature workflow remains explicit and composable. @fn FNatureState ReduceNatureSeeded( const FNatureState &State, const rtk::PayloadAction<TArray<FFeatureSeed>> &Action) */
FNatureState ReduceNatureSeeded(
    const FNatureState &State,
    const rtk::PayloadAction<TArray<FFeatureSeed>> &Action) {
  return (func::pipe(State) | [&](FNatureState Next) -> FNatureState {
  Next.Items =
      NatureAdapters::NatureAdapter().setAll(State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

} // namespace NatureReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Nature/SystemsNatureActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureSlice {

/** User Story: As a features systems nature consumer, I need to invoke get slice through a stable signature so the features systems nature workflow remains explicit and composable. @fn const rtk::Slice<FNatureState> &GetSlice() */
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
} // namespace Game
} // namespace ForbocAI


#include "Features/Systems/Nature/Entity/EntityAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureFactories {

/** User Story: As a features systems nature consumer, I need to invoke create initial state through a stable signature so the features systems nature workflow remains explicit and composable. @fn FNatureState CreateInitialState() */
FNatureState CreateInitialState() {
  FNatureState State;
  State.Items = NatureAdapters::NatureAdapter().getInitialState();
  return State;
}

} // namespace NatureFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
