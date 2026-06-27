#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"

#include "Features/Systems/Bots/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonSelectors {
namespace {

FString SelectInteractionPrompt(const FString &Prompt) {
  return Prompt.IsEmpty() ? FString(TEXT("Press E to talk")) : Prompt;
}

FString SelectDefaultPlayerLine(const FString &PlayerLine) {
  return PlayerLine.IsEmpty() ? FString(TEXT("What should I know here?"))
                              : PlayerLine;
}

bool SelectOverlapMatches(const FTownspersonInteractionOverlapRequest &Request) {
  return Request.bOtherActorPresent && !Request.bOtherActorIsSelf &&
         Request.bOtherActorIsPawn;
}

} // namespace

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

FTownspersonViewDefaults SelectViewDefaults(
    const FTownspersonViewDefaultsRequest &Request) {
  return {SelectInteractionPrompt(Request.InteractionPrompt),
          SelectDefaultPlayerLine(Request.DefaultPlayerLine)};
}

FTownspersonInteractionOverlapViewModel SelectInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request) {
  const bool bMatches = SelectOverlapMatches(Request);
  FTownspersonInteractionOverlapViewModel Model;
  Model.bShouldApply = bMatches;
  Model.bPlayerNearby =
      bMatches ? Request.bEntering : Request.bCurrentPlayerNearby;
  Model.bPromptVisible = Model.bPlayerNearby;
  return Model;
}

} // namespace TownspersonSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
