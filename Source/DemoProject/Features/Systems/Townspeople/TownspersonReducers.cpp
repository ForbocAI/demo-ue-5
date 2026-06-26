#include "Features/Systems/Townspeople/TownspersonReducers.h"

#include "Features/Systems/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonReducers {

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action) {
  FTownspersonState Next = State;
  Next.Items = TownspersonAdapters::TownspersonAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
}

} // namespace TownspersonReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
