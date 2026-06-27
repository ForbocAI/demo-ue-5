#include "Features/Systems/Nature/NatureReducers.h"

#include "Features/Systems/Nature/NatureEntityAdapters.h"

namespace ForbocAI {
namespace Demo {
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
} // namespace Demo
} // namespace ForbocAI
