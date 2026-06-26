#include "Features/Systems/Nature/NatureReducers.h"

#include "Features/Systems/Nature/NatureAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace NatureReducers {

FNatureState ReduceNatureSeeded(
    const FNatureState &State,
    const rtk::Action<TArray<FNatureFeatureSeed>> &Action) {
  FNatureState Next = State;
  Next.Items =
      NatureAdapters::NatureAdapter().setAll(State.Items, Action.PayloadValue);
  return Next;
}

} // namespace NatureReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
