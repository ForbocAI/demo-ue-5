#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Townspeople/TownspeopleAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonReducers {
namespace {

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce interaction prompt through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FString ReduceInteractionPrompt( const FTownspersonViewDefaultsReduceRequest &Request) */
FString ReduceInteractionPrompt(
    const FTownspersonViewDefaultsReduceRequest &Request) {
  return Request.Request.InteractionPrompt.IsEmpty()
             ? Request.Defaults.InteractionPrompt
             : Request.Request.InteractionPrompt;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce default player line through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FString ReduceDefaultPlayerLine( const FTownspersonViewDefaultsReduceRequest &Request) */
FString ReduceDefaultPlayerLine(
    const FTownspersonViewDefaultsReduceRequest &Request) {
  return Request.Request.DefaultPlayerLine.IsEmpty()
             ? Request.Defaults.DefaultPlayerLine
             : Request.Request.DefaultPlayerLine;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce overlap matches through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn bool ReduceOverlapMatches( const FTownspersonInteractionOverlapRequest &Request) */
bool ReduceOverlapMatches(
    const FTownspersonInteractionOverlapRequest &Request) {
  return Request.bOtherActorPresent && !Request.bOtherActorIsSelf &&
         Request.bOtherActorIsPawn;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce interaction intent projections through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonState ReduceInteractionIntentProjections( const FTownspersonState &State, const TArray<FTownspersonSeed> &Townspeople) */
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

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonState ReduceTownspeopleSeeded( const FTownspersonState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FTownspersonState Next) -> FTownspersonState {
  Next.Items = TownspersonAdapters::TownspersonAdapter().setAll(
      State.Items, Action.PayloadValue);
  return ReduceInteractionIntentProjections(Next, Action.PayloadValue);
  }).val;
}

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce view defaults through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonViewDefaults ReduceViewDefaults( const ForbocAI::Game::Data::FTownspersonDefaultsSettings &Settings) */
FTownspersonViewDefaults ReduceViewDefaults(
    const ForbocAI::Game::Data::FTownspersonDefaultsSettings &Settings) {
  return {Settings.Id, Settings.Name, Settings.Role, Settings.Persona,
          Settings.InteractionPrompt, Settings.DefaultPlayerLine};
}

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce view defaults through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonViewDefaults ReduceViewDefaults( const FTownspersonViewDefaultsReduceRequest &Request) */
FTownspersonViewDefaults ReduceViewDefaults(
    const FTownspersonViewDefaultsReduceRequest &Request) {
  return {Request.Defaults.Id,
          Request.Defaults.Name,
          Request.Defaults.Role,
          Request.Defaults.Persona,
          ReduceInteractionPrompt(Request),
          ReduceDefaultPlayerLine(Request)};
}

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce interaction overlap through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonInteractionOverlapViewModel ReduceInteractionOverlap( const FTownspersonInteractionOverlapRequest &Request) */
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

/** User Story: As a systems bots townspeople consumer, I need to invoke reduce by interaction intent through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn TArray<FTownspersonSeed> ReduceByInteractionIntent( const FTownspersonIntentProjectionRequest &Request) */
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

/** User Story: As a systems bots townspeople consumer, I need to invoke get slice through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::Slice<FTownspersonState> &GetSlice() */
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

/** User Story: As a systems bots townspeople consumer, I need to invoke create initial state through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn FTownspersonState CreateInitialState() */
FTownspersonState CreateInitialState() {
  FTownspersonState State;
  State.Items = TownspersonAdapters::TownspersonAdapter().getInitialState();
  return State;
}

} // namespace TownspersonFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
