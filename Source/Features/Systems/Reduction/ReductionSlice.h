#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Bot/BotSlice.h"
#include "Features/Systems/Interaction/Conversation/InteractionConversationSlice.h"

#include "Core/frmt.hpp"
#include "Core/ecs.hpp"
#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Systems/Bots/AI/AIActions.h"
#include "Features/Systems/Bots/SystemsBotsActions.h"
#include "Features/Systems/Bots/Goals/GoalsActions.h"
#include "Features/Systems/Bots/Goals/GoalsSlice.h"
#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/Bots/Stats/StatsActions.h"
#include "Features/Systems/Bots/Stats/StatsSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/Interaction/InteractionActions.h"
#include "Features/Systems/Landmarks/SystemsLandmarksActions.h"
#include "Features/Systems/Level/SystemsLevelSlice.h"
#include "Features/Systems/Nature/SystemsNatureActions.h"
#include "Features/Systems/Rendering/SystemsRenderingSlice.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainSlice.h"
#include "Features/Systems/UI/Chat/Rendering/ChatRenderingSlice.h"
#include "Features/Systems/UI/Chat/ChatSlice.h"
#include "Features/Systems/UI/Conversation/UIConversationSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {
namespace {

typedef TFunction<func::Maybe<FRuntimeState>(
    const FRuntimeState &, const rtk::AnyAction &)>
    FRuntimeActionReducer;

template <typename Payload>
using TRuntimePayloadReducer =
    FRuntimeState (*)(const FRuntimeState &,
                      const rtk::PayloadAction<Payload> &);

struct FRuntimeActionReducerDeclaration {
  FRuntimeActionReducer Reduce;
};

struct FRuntimeActionReducerPlan {};

template <typename Catalog> struct TRuntimeReducerRegistry;

struct FRuntimeActionReducerRequest {
  const FRuntimeState &State;
  const rtk::AnyAction &Action;
};

/** User Story: As a features systems reduction consumer, I need to invoke reduce extracted payload action through a stable signature so the features systems reduction workflow remains explicit and composable. @fn template <typename Payload> func::Maybe<FRuntimeState> ReduceExtractedPayloadAction( FRuntimeState State, const rtk::PayloadAction<Payload> &PayloadAct, TRuntimePayloadReducer<Payload> Reduce) */
template <typename Payload>
func::Maybe<FRuntimeState> ReduceExtractedPayloadAction(
    FRuntimeState State,
    const rtk::PayloadAction<Payload> &PayloadAct,
    TRuntimePayloadReducer<Payload> Reduce) {
  return func::just(Reduce(State, PayloadAct));
}

/** User Story: As a features systems reduction consumer, I need to invoke runtime payload reducer through a stable signature so the features systems reduction workflow remains explicit and composable. @fn template <typename Payload> FRuntimeActionReducerDeclaration RuntimePayloadReducer( const rtk::ActionCreator<Payload> &Creator, TRuntimePayloadReducer<Payload> Reduce) */
template <typename Payload>
FRuntimeActionReducerDeclaration RuntimePayloadReducer(
    const rtk::ActionCreator<Payload> &Creator,
    TRuntimePayloadReducer<Payload> Reduce) {
  return {[Creator, Reduce](const FRuntimeState &State,
                            const rtk::AnyAction &Action) {
    return func::mbind(
        Creator.extract(Action),
        [&State, &Action, Reduce](const Payload &PayloadValue) {
          return ReduceExtractedPayloadAction(
              State,
              rtk::PayloadAction<Payload>{Action.Type, PayloadValue},
              Reduce);
        });
  }};
}

/** User Story: As a features systems reduction consumer, I need to invoke reduce runtime action maybe through a stable signature so the features systems reduction workflow remains explicit and composable. @fn func::Maybe<FRuntimeState> ReduceRuntimeActionMaybe( const FRuntimeActionReducerRequest &Request, const TArray<FRuntimeActionReducerDeclaration> &Declarations) */
func::Maybe<FRuntimeState> ReduceRuntimeActionMaybe(
    const FRuntimeActionReducerRequest &Request,
    const TArray<FRuntimeActionReducerDeclaration> &Declarations) {
  return func::fold_array<FRuntimeActionReducerDeclaration,
                          func::Maybe<FRuntimeState>>(
      Declarations, func::nothing<FRuntimeState>(),
      [&Request](const func::Maybe<FRuntimeState> &Current,
                 const FRuntimeActionReducerDeclaration &Declaration) {
        return func::match(
            Current,
            [](const FRuntimeState &Reduced) {
              return func::just(Reduced);
            },
            [&Request, &Declaration]() {
              return Declaration.Reduce(Request.State, Request.Action);
            });
      });
}

} // namespace

#include "Features/Systems/Bot/BotSlice.h"

namespace {

template <> struct TRuntimeReducerRegistry<FRuntimeActionReducerPlan> {
  /** User Story: As a features systems reduction consumer, I need to invoke declarations through a stable signature so the features systems reduction workflow remains explicit and composable. @fn static const TArray<FRuntimeActionReducerDeclaration> &Declarations() */
  static const TArray<FRuntimeActionReducerDeclaration> &Declarations() {
    static const TArray<FRuntimeActionReducerDeclaration>
        RegisteredDeclarations = {
            RuntimePayloadReducer(
                RuntimeActions::TownspersonInteractionSourceObserved(),
                ReduceTownspersonInteractionSourceObserved),
            RuntimePayloadReducer(TownspersonActions::TownspeopleSeeded(),
                                  ReduceRuntimeTownspeopleSeeded),
            RuntimePayloadReducer(HorseActions::HorsesSeeded(),
                                  ReduceRuntimeHorsesSeeded)};
    return RegisteredDeclarations;
  }
};

} // namespace

/** User Story: As a features systems reduction consumer, I need to invoke reduce runtime action through a stable signature so the features systems reduction workflow remains explicit and composable. @fn FRuntimeState ReduceRuntimeAction(const FRuntimeState &State, const rtk::AnyAction &Action) */
FRuntimeState ReduceRuntimeAction(const FRuntimeState &State,
                                  const rtk::AnyAction &Action) {
  return func::or_else(
      ReduceRuntimeActionMaybe(
          {State, Action},
          TRuntimeReducerRegistry<
              FRuntimeActionReducerPlan>::Declarations()),
      State);
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
