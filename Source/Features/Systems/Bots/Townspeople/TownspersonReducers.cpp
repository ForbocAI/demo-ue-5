#include "Features/Systems/Bots/Townspeople/TownspersonReducers.h"

#include "Features/Systems/Bots/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonReducers {
namespace {

FString ReduceInteractionPrompt(const FString &Prompt) {
  return Prompt.IsEmpty() ? FString(TEXT("Press E to talk")) : Prompt;
}

FString ReduceDefaultPlayerLine(const FString &PlayerLine) {
  return PlayerLine.IsEmpty() ? FString(TEXT("What should I know here?"))
                              : PlayerLine;
}

bool ReduceOverlapMatches(
    const FTownspersonInteractionOverlapRequest &Request) {
  return Request.bOtherActorPresent && !Request.bOtherActorIsSelf &&
         Request.bOtherActorIsPawn;
}

TArray<FTownspersonSeed> ReduceByInteractionIntentAtIndex(
    const FTownspersonIntentProjectionRequest &Request, int32 Index,
    TArray<FTownspersonSeed> Acc) {
  return Index >= Request.Townspeople.Num()
             ? Acc
             : (Request.Townspeople[Index].InteractionIntent == Request.Intent
                    ? Acc.Add(Request.Townspeople[Index])
                    : 0,
                ReduceByInteractionIntentAtIndex(Request, Index + 1, Acc));
}

FTownspersonState ReduceInteractionIntentProjections(
    const FTownspersonState &State,
    const TArray<FTownspersonSeed> &Townspeople) {
  return (func::pipe(State) |
          [&Townspeople](FTownspersonState Next) -> FTownspersonState {
            Next.GeneralTownspeople = ReduceByInteractionIntent(
                {Townspeople, ETownspersonInteractionIntent::General});
            Next.DialogueTownspeople = ReduceByInteractionIntent(
                {Townspeople, ETownspersonInteractionIntent::Dialogue});
            Next.MemoryTownspeople = ReduceByInteractionIntent(
                {Townspeople, ETownspersonInteractionIntent::Memory});
            Next.CombatValidationTownspeople = ReduceByInteractionIntent(
                {Townspeople,
                 ETownspersonInteractionIntent::CombatValidation});
            return Next;
          })
      .val;
}

} // namespace

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FTownspersonState Next) -> FTownspersonState {
  Next.Items = TownspersonAdapters::TownspersonAdapter().setAll(
      State.Items, Action.PayloadValue);
  return ReduceInteractionIntentProjections(Next, Action.PayloadValue);
  }).val;
}

FTownspersonState ReduceViewDefaultsRequested(
    const FTownspersonState &State,
    const rtk::PayloadAction<FTownspersonViewDefaultsRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FTownspersonState Next) -> FTownspersonState {
            Next.LastViewDefaults = ReduceViewDefaults(Action.PayloadValue);
            return Next;
          })
      .val;
}

FTownspersonState ReduceInteractionOverlapObserved(
    const FTownspersonState &State,
    const rtk::PayloadAction<FTownspersonInteractionOverlapRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FTownspersonState Next) -> FTownspersonState {
            Next.LastInteractionOverlap =
                ReduceInteractionOverlap(Action.PayloadValue);
            return Next;
          })
      .val;
}

FTownspersonViewDefaults ReduceViewDefaults(
    const FTownspersonViewDefaultsRequest &Request) {
  return {ReduceInteractionPrompt(Request.InteractionPrompt),
          ReduceDefaultPlayerLine(Request.DefaultPlayerLine)};
}

FTownspersonInteractionOverlapViewModel ReduceInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request) {
  const bool bMatches = ReduceOverlapMatches(Request);
  FTownspersonInteractionOverlapViewModel Model;
  Model.bShouldApply = bMatches;
  Model.bPlayerNearby =
      bMatches ? Request.bEntering : Request.bCurrentPlayerNearby;
  Model.bPromptVisible = Model.bPlayerNearby;
  return Model;
}

TArray<FTownspersonSeed> ReduceByInteractionIntent(
    const FTownspersonIntentProjectionRequest &Request) {
  return ReduceByInteractionIntentAtIndex(Request, 0, {});
}

} // namespace TownspersonReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
