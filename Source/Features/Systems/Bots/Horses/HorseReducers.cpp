#include "Features/Systems/Bots/Horses/HorseReducers.h"

#include "Features/Systems/Bots/Horses/HorseAdapters.h"

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
