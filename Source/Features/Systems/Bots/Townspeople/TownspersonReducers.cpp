#include "Features/Systems/Bots/Townspeople/TownspersonReducers.h"

#include "Features/Systems/Bots/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonReducers {

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FTownspersonState Next) -> FTownspersonState {
  Next.Items = TownspersonAdapters::TownspersonAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

} // namespace TownspersonReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
