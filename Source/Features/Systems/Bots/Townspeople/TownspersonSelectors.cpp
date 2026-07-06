#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"

#include "Features/Systems/Bots/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonSelectors {

TArray<FTownspersonSeed> SelectAll(const FTownspersonState &State) {
  return (func::pipe(State.Items) |
         [](const rtk::EntityState<FTownspersonSeed> &Items)
             -> TArray<FTownspersonSeed> {
    return TownspersonAdapters::TownspersonAdapter().getSelectors().selectAll(
        Items);
  }).val;
}

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

const TArray<FTownspersonSeed> &
SelectGeneralTownspeople(const FTownspersonState &State) {
  return State.GeneralTownspeople;
}

const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FTownspersonState &State) {
  return State.DialogueTownspeople;
}

const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FTownspersonState &State) {
  return State.MemoryTownspeople;
}

const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FTownspersonState &State) {
  return State.CombatValidationTownspeople;
}

const FTownspersonViewDefaults &
SelectLastViewDefaults(const FTownspersonState &State) {
  return State.LastViewDefaults;
}

const FTownspersonInteractionOverlapViewModel &
SelectLastInteractionOverlap(const FTownspersonState &State) {
  return State.LastInteractionOverlap;
}

} // namespace TownspersonSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
