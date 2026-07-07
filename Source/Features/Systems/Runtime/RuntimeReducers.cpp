#include "Features/Systems/Runtime/RuntimeReducers.h"

#include "Features/Systems/Runtime/RuntimeBotReducers.h"
#include "Features/Systems/Runtime/RuntimeInteractionReducers.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"

#include "Core/frmt.hpp"
#include "Core/ecs.hpp"
#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Systems/Bots/AI/BotAIActions.h"
#include "Features/Systems/Bots/BotActions.h"
#include "Features/Systems/Bots/Goals/BotGoalActions.h"
#include "Features/Systems/Bots/Goals/BotGoalReducers.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Horses/HorseSelectors.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Bots/Stats/BotStatsActions.h"
#include "Features/Systems/Bots/Stats/BotStatsReducers.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"
#include "Features/Systems/Dialogue/DialogueReducers.h"
#include "Features/Systems/Interaction/InteractionActions.h"
#include "Features/Systems/Landmarks/LandmarkActions.h"
#include "Features/Systems/Landmarks/LandmarkSelectors.h"
#include "Features/Systems/Level/LevelReducers.h"
#include "Features/Systems/Nature/NatureActions.h"
#include "Features/Systems/Nature/NatureSelectors.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainReducers.h"
#include "Features/Systems/UI/UISharedReducers.h"
#include "Features/Systems/UI/UIChatReducers.h"
#include "Features/Systems/UI/UIConversationReducers.h"

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

struct FRuntimeActionReducerCatalog {};

template <typename Catalog> struct TRuntimeReducerRegistry;

struct FRuntimeActionReducerRequest {
  const FRuntimeState &State;
  const rtk::AnyAction &Action;
};

template <typename Payload>
func::Maybe<FRuntimeState> ReduceExtractedPayloadAction(
    FRuntimeState State,
    const rtk::PayloadAction<Payload> &PayloadAct,
    TRuntimePayloadReducer<Payload> Reduce) {
  return func::just(Reduce(State, PayloadAct));
}

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

#include "Features/Systems/Runtime/RuntimeBotReducers.h"

namespace {

template <> struct TRuntimeReducerRegistry<FRuntimeActionReducerCatalog> {
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

FRuntimeState ReduceRuntimeAction(const FRuntimeState &State,
                                  const rtk::AnyAction &Action) {
  return func::or_else(
      ReduceRuntimeActionMaybe(
          {State, Action},
          TRuntimeReducerRegistry<
              FRuntimeActionReducerCatalog>::Declarations()),
      State);
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
