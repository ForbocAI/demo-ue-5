#include "Features/Systems/Bots/Townspeople/TownspeopleSelectors.h"

#include "Features/Systems/Bots/Townspeople/TownspeopleAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonSelectors {

/** User Story: As a systems bots townspeople consumer, I need to invoke select all through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn TArray<FTownspersonSeed> SelectAll(const FTownspersonState &State) */
TArray<FTownspersonSeed> SelectAll(const FTownspersonState &State) {
  return (func::pipe(State.Items) |
         [](const rtk::EntityState<FTownspersonSeed> &Items)
             -> TArray<FTownspersonSeed> {
    return TownspersonAdapters::TownspersonAdapter().getSelectors().selectAll(
        Items);
  }).val;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke select by id through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn func::Maybe<FTownspersonSeed> SelectById(const FTownspersonState &State, const FString &Id) */
func::Maybe<FTownspersonSeed> SelectById(const FTownspersonState &State,
                                         const FString &Id) {
  return (func::pipe(State.Items) |
          [&Id](const rtk::EntityState<FTownspersonSeed> &Items)
              -> func::Maybe<FTownspersonSeed> {
            return TownspersonAdapters::TownspersonAdapter()
                .getSelectors()
                .selectById(Items, Id);
          })
      .val;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke select general townspeople through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectGeneralTownspeople(const FTownspersonState &State) */
const TArray<FTownspersonSeed> &
SelectGeneralTownspeople(const FTownspersonState &State) {
  return State.GeneralTownspeople;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke select dialogue townspeople through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectDialogueTownspeople(const FTownspersonState &State) */
const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FTownspersonState &State) {
  return State.DialogueTownspeople;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke select memory townspeople through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectMemoryTownspeople(const FTownspersonState &State) */
const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FTownspersonState &State) {
  return State.MemoryTownspeople;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke select combat validation townspeople through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectCombatValidationTownspeople(const FTownspersonState &State) */
const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FTownspersonState &State) {
  return State.CombatValidationTownspeople;
}

} // namespace TownspersonSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
