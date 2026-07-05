#include "Features/Systems/Runtime/RuntimeReducers.h"

#include "Core/frmt.hpp"
#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Goals/BotGoalReducers.h"
#include "Features/Systems/Bots/Horses/HorseSelectors.h"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Stats/BotStatsReducers.h"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"
#include "Features/Systems/Dialogue/DialogueReducers.h"
#include "Features/Systems/Landmarks/LandmarkSelectors.h"
#include "Features/Systems/Level/LevelReducers.h"
#include "Features/Systems/Nature/NatureSelectors.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Terrain/TerrainReducers.h"
#include "Features/Systems/UI/UIReducers.h"

namespace ForbocAI {
namespace Demo {
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

TArray<FVector> ReduceWorldRoute(const TArray<FLevelLocalPoint> &Route,
                                 const FLevelTerrainData *TerrainData) {
  return func::match(
      func::from_nullable(TerrainData),
      [&Route](const FLevelTerrainData &Terrain) {
        return LevelSystemReducers::BuildWorldRoute({Route, Terrain});
      },
      []() { return TArray<FVector>(); });
}

FRuntimeLevelViewPayload ReduceEmptyLevelViewPayload() {
  return FRuntimeLevelViewPayload();
}

void ReduceAppendSection(TArray<FLevelRuntimeSectionSpawn> &Sections,
                         const FLevelRuntimeSectionSpawn &Section) {
  Sections.Add(Section);
}

template <typename Payload>
func::Maybe<FRuntimeState> ReduceExtractedPayloadAction(
    const FRuntimeState &State, const rtk::AnyAction &Action,
    const Payload &PayloadValue,
    TRuntimePayloadReducer<Payload> Reduce) {
  return func::just(
      Reduce(State, rtk::PayloadAction<Payload>{Action.Type, PayloadValue}));
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
          return ReduceExtractedPayloadAction(State, Action, PayloadValue,
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

FRuntimeState ReduceRuntimeProjected(const FRuntimeState &State) {
  return (func::pipe(State) |
          [](FRuntimeState Next) -> FRuntimeState {
            Next.Ecs.World =
                SystemsAdapters::ProjectRuntimeWorld({Next});
            return Next;
          })
      .val;
}

FRuntimeTownspersonInteractionRequest
ReduceTownspersonInteractionRequest(
    const FRuntimeTownspersonInteractionSource &Source) {
  return {Source.Name, Source.Role, Source.Persona, Source.DefaultPlayerLine,
          Source.PinnedResponse};
}

FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request) {
  return {Request.Seed.Id,
          Request.Seed.Name,
          Request.Seed.Role,
          Request.Seed.Persona,
          Request.Seed.InteractionPrompt,
          Request.Seed.DefaultPlayerLine,
          Request.Seed.PinnedResponse,
          ReduceWorldRoute(Request.Seed.PatrolRoute, Request.TerrainData)};
}

FRuntimeHorseViewSpawn
ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request) {
  return {Request.Seed.Name,
          ReduceWorldRoute(Request.Seed.PatrolRoute, Request.TerrainData),
          Request.Seed.bMountedRider};
}

FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request) {
  return {Request.Name, Request.Role, Request.Persona, Request.PlayerLine,
          Request.PinnedResponse};
}

FUIPayload ReduceConversationPresentedPayload(
    const FDialogueReplyPayload &DialogueReply,
    const ForbocAI::Demo::Data::FUIRuntimeSettings &UISettings) {
  FUIPayload Payload;
  Payload.Id = frmt::RuntimeString(
      UISettings.PayloadIdFormat,
      frmt::Args(
          {frmt::Arg(DialogueReply.Request.Name)}));
  Payload.Conversation = UIReducers::ReduceRuntimeConversationViewModel(
      {DialogueReply.Request.Name, DialogueReply.Request.Role,
       DialogueReply.Request.PlayerLine, DialogueReply.Reply},
      UISettings);
  return Payload;
}

FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload(
    const FRuntimeTownspersonInteractionPayloadRequest &Request) {
  const FDialogueReplyPayload DialogueReply =
      DialogueReducers::ReduceLocalReplyPayload(
          ReduceLocalDialogueReplyRequest(Request.Request),
          Request.DialogueSettings);
  return {DialogueReply,
          ReduceConversationPresentedPayload(DialogueReply,
                                             Request.UISettings)};
}

FRuntimeLevelViewPayload ReduceLevelViewPayload(
    const FRuntimeState &State,
    const FRuntimeLevelViewPayloadRequest &Request) {
  check(Request.TerrainData);
  check(Request.OrthoData);
  check(Request.RuntimeLayout);
  check(Request.Geometry);

  FRuntimeLevelViewPayload Payload;
  Payload.TerrainMesh = TerrainReducers::BuildTerrainMeshPayload(
      *Request.TerrainData, *Request.OrthoData, *Request.Geometry);
  Payload.bTerrainMeshLoaded = Payload.TerrainMesh.bLoaded;

  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildRuntimeSectionSpawn(
                          {Request.RuntimeLayout->Terrain,
                           *Request.TerrainData, *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildNatureSectionSpawn(
                          {NatureSelectors::SelectAll(State.Nature),
                           *Request.TerrainData, *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildLandmarkSectionSpawn(
                          {LandmarkSelectors::SelectAll(State.Landmarks),
                           *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildRuntimeSectionSpawn(
                          {Request.RuntimeLayout->Town,
                           *Request.TerrainData, *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildRuntimeSectionSpawn(
                          {Request.RuntimeLayout->Mine,
                           *Request.TerrainData, *Request.Geometry}));
  ReduceAppendSection(Payload.Sections,
                      LevelSystemReducers::BuildOverlaySectionSpawn(
                          {*Request.RuntimeLayout, *Request.TerrainData,
                           *Request.Geometry}));

  Payload.Townspeople =
      func::map_array<FTownspersonSeed, FRuntimeTownspersonViewSpawn>(
          TownspersonSelectors::SelectAll(State.Townspeople),
          [&Request](const FTownspersonSeed &Seed) {
             return ReduceTownspersonViewSpawn({Seed, Request.TerrainData});
          });
  Payload.Horses =
      func::map_array<FHorseRouteSeed, FRuntimeHorseViewSpawn>(
          HorseSelectors::SelectAll(State.Horses),
          [&Request](const FHorseRouteSeed &Seed) {
             return ReduceHorseViewSpawn({Seed, Request.TerrainData});
          });
  return Payload;
}

FRuntimeState ReduceTownspersonInteractionSourceObserved(
    const FRuntimeState &State,
    const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState Next) -> FRuntimeState {
            Next.LastTownspersonInteractionRequest =
                ReduceTownspersonInteractionRequest(Action.PayloadValue);
            const FRuntimeTownspersonInteractionPayload Payload =
                ReduceTownspersonInteractionPayload(
                    {Next.LastTownspersonInteractionRequest,
                     Next.Dialogue.RuntimeSettings, Next.UI.RuntimeSettings});
            Next.Dialogue = DialogueReducers::ReduceLocalReplyResolved(
                Next.Dialogue, {Payload.DialogueReply.Id,
                                Payload.DialogueReply});
            Next.UI = UIReducers::ReduceConversationPresented(
                Next.UI, {Payload.UI.Id, Payload.UI});
            return Next;
          })
      .val;
}

FRuntimeState ReduceRuntimeTownspeopleSeeded(
    const FRuntimeState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState Next) -> FRuntimeState {
            Next.BotStats = BotStatsReducers::ReduceTownspeopleSeeded(
                {Next.BotStats, Action.PayloadValue, Next.BotRuntime});
            Next.BotGoals = BotGoalReducers::ReduceTownspeopleSeeded(
                {Next.BotGoals, Action.PayloadValue, Next.BotRuntime});
            return Next;
          })
      .val;
}

FRuntimeState ReduceRuntimeHorsesSeeded(
    const FRuntimeState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState Next) -> FRuntimeState {
            Next.BotStats = BotStatsReducers::ReduceHorsesSeeded(
                {Next.BotStats, Action.PayloadValue, Next.BotRuntime});
            Next.BotGoals = BotGoalReducers::ReduceHorsesSeeded(
                {Next.BotGoals, Action.PayloadValue, Next.BotRuntime});
            return Next;
          })
      .val;
}

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
} // namespace Demo
} // namespace ForbocAI
