#include "Features/Systems/Nature/SystemsNatureSlice.h"

#include "Features/Systems/Nature/Entity/EntityAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureReducers {

FNatureState ReduceNatureSeeded(
    const FNatureState &State,
    const rtk::PayloadAction<TArray<FNatureFeatureSeed>> &Action) {
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

FNatureState CreateInitialState() {
  FNatureState State;
  State.Items = NatureAdapters::NatureAdapter().getInitialState();
  return State;
}

} // namespace NatureFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
