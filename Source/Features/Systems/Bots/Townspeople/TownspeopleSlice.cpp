#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Townspeople/TownspeopleAdapters.h"

namespace ForbocAI {
namespace Game {
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

FTownspersonViewDefaults ReduceViewDefaults(
    const ForbocAI::Game::Data::FTownspersonDefaultsSettings &Settings) {
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
  return func::filter_array<FTownspersonSeed>(
      Request.Townspeople, [&Request](const FTownspersonSeed &Townsperson) {
        return Townsperson.InteractionIntent == Request.Intent;
      });
}

} // namespace TownspersonReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonSlice {

const rtk::Slice<FTownspersonState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FTownspersonState>> Slice =
      func::lazy([]() -> rtk::Slice<FTownspersonState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FTownspersonState>(
          TEXT("townspeople"), TownspersonFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FTownspersonState> &Builder) {
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                TownspersonReducers::ReduceTownspeopleSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace TownspersonSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI


#include "Features/Systems/Bots/Townspeople/TownspeopleAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonFactories {

FTownspersonState CreateInitialState() {
  FTownspersonState State;
  State.Items = TownspersonAdapters::TownspersonAdapter().getInitialState();
  return State;
}

} // namespace TownspersonFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
