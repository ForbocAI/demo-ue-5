#include "Features/Systems/Horses/HorseReducers.h"

#include "Features/Systems/Horses/HorseAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseReducers {

FHorseState ReduceHorsesSeeded(
    const FHorseState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action) {
  FHorseState Next = State;
  Next.Items =
      HorseAdapters::HorseAdapter().setAll(State.Items, Action.PayloadValue);
  return Next;
}

} // namespace HorseReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
