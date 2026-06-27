#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"

#include "Features/Systems/Bots/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
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

TArray<FTownspersonSeed> SelectByInteractionIntent(
    const FTownspersonState &State, ETownspersonInteractionIntent Intent) {
  return (func::pipe(SelectAll(State)) |
          [Intent](const TArray<FTownspersonSeed> &Townspeople)
              -> TArray<FTownspersonSeed> {
            TArray<FTownspersonSeed> Matches;
            for (const FTownspersonSeed &Townsperson : Townspeople) {
              if (Townsperson.InteractionIntent == Intent) {
                Matches.Add(Townsperson);
              }
            }
            return Matches;
          })
      .val;
}

} // namespace TownspersonSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
