#include "Features/Systems/Bots/Townspeople/TownspersonReducers.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Townspeople/TownspersonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonReducers {
namespace {

FString ReduceInteractionPrompt(
    const FTownspersonViewDefaultsReduceRequest &Request) {
  return Request.Request.InteractionPrompt.IsEmpty()
             ? Request.Defaults.InteractionPrompt
             : Request.Request.InteractionPrompt;
}

FString ReduceDefaultPlayerLine(
    const FTownspersonViewDefaultsReduceRequest &Request) {
  return Request.Request.DefaultPlayerLine.IsEmpty()
             ? Request.Defaults.DefaultPlayerLine
             : Request.Request.DefaultPlayerLine;
}

bool ReduceOverlapMatches(
    const FTownspersonInteractionOverlapRequest &Request) {
  return Request.bOtherActorPresent && !Request.bOtherActorIsSelf &&
         Request.bOtherActorIsPawn;
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
            Next.LastViewDefaults =
                ReduceViewDefaults(
                    {Action.PayloadValue, Next.LastViewDefaults});
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
    const ForbocAI::Demo::Data::FTownspersonDefaultsSettings &Settings) {
  return {Settings.Id, Settings.Name, Settings.Role, Settings.Persona,
          Settings.InteractionPrompt, Settings.DefaultPlayerLine};
}

FTownspersonViewDefaults ReduceViewDefaults(
    const FTownspersonViewDefaultsReduceRequest &Request) {
  return {Request.Defaults.Id,
          Request.Defaults.Name,
          Request.Defaults.Role,
          Request.Defaults.Persona,
          ReduceInteractionPrompt(Request),
          ReduceDefaultPlayerLine(Request)};
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
  return ecs::filterArray<FTownspersonSeed>(
      Request.Townspeople, [&Request](const FTownspersonSeed &Townsperson) {
        return Townsperson.InteractionIntent == Request.Intent;
      });
}

} // namespace TownspersonReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
