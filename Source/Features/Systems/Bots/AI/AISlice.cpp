#include "Features/Systems/Bots/AI/AISlice.h"

#include "Features/Systems/Bots/AI/AIAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIReducers {

/** User Story: As a systems bots ai consumer, I need to invoke reduce bot aiseeded through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState ReduceBotAISeeded( const FBotAIState &State, const rtk::PayloadAction<TArray<FBotAIComponent>> &Action) */
FBotAIState ReduceBotAISeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FBotAIComponent>> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  Next.Items =
      BotAIAdapters::BotAIAdapter().setAll(State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

/** User Story: As a systems bots ai consumer, I need to invoke reduce bot aiupdated through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState ReduceBotAIUpdated(const FBotAIState &State, const rtk::PayloadAction<FBotAIUpdated> &Action) */
FBotAIState ReduceBotAIUpdated(const FBotAIState &State,
                               const rtk::PayloadAction<FBotAIUpdated> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  const FBotAIUpdated Payload = Action.PayloadValue;
  Next.Items = BotAIAdapters::BotAIAdapter().updateOne(
      State.Items, Payload.Id,
      [Payload](const FBotAIComponent &Current) -> FBotAIComponent {
        FBotAIComponent Updated = Current;
        Updated.BehaviorState = Payload.BehaviorState;
        Updated.TargetEntityId = Payload.TargetEntityId;
        Updated.TargetLocation = Payload.TargetLocation;
        Updated.bHasTargetLocation = Payload.bHasTargetLocation;
        Updated.PatrolIndex = Payload.PatrolIndex;
        return Updated;
      });
  return Next;
  }).val;
}

/** User Story: As a systems bots ai consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState ReduceTownspeopleSeeded( const FBotAIState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FBotAIState ReduceTownspeopleSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  Next.Items = BotAIAdapters::BotAIAdapter().upsertMany(
      State.Items, BotAIFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

/** User Story: As a systems bots ai consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState ReduceHorsesSeeded( const FBotAIState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
FBotAIState ReduceHorsesSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  Next.Items = BotAIAdapters::BotAIAdapter().upsertMany(
      State.Items, BotAIFactories::FromHorses(Action.PayloadValue));
  return Next;
  }).val;
}

} // namespace BotAIReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAISlice {

/** User Story: As a systems bots ai consumer, I need to invoke get slice through a stable signature so the systems bots ai workflow remains explicit and composable. @fn const rtk::Slice<FBotAIState> &GetSlice() */
const rtk::Slice<FBotAIState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotAIState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotAIState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotAIState>(
      TEXT("botAI"), BotAIFactories::CreateInitialState(),
      [](rtk::ActionReducerMapBuilder<FBotAIState> &Builder) {
    Builder.addCase(BotAIActions::BotAISeeded(),
                                BotAIReducers::ReduceBotAISeeded);
    Builder.addCase(BotAIActions::BotAIUpdated(),
                                BotAIReducers::ReduceBotAIUpdated);
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                BotAIReducers::ReduceTownspeopleSeeded);
    Builder.addCase(HorseActions::HorsesSeeded(),
                                BotAIReducers::ReduceHorsesSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace BotAISlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI


#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Source/Mapping/MappingAdapters.h"
#include "Features/Systems/Bots/AI/AIAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIFactories {
namespace {

/** User Story: As a systems bots ai consumer, I need to invoke patrol aisource through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAISource PatrolAISource(const FString &Id, const TArray<FLevelLocalPoint> &PatrolRoute) */
FBotAISource PatrolAISource(const FString &Id,
                            const TArray<FLevelLocalPoint> &PatrolRoute) {
  return {Id,
          EBotBehaviorState::Patrol,
          FString(),
          BotSourceMappingAdapters::FirstRoutePoint(PatrolRoute),
          true,
          0,
          PatrolRoute};
}

/** User Story: As a systems bots ai consumer, I need to invoke route aisource through a stable signature so the systems bots ai workflow remains explicit and composable. @fn template <typename Seed> FBotAISource RouteAISource(const Seed &SeedValue) */
template <typename Seed> FBotAISource RouteAISource(const Seed &SeedValue) {
  return PatrolAISource(SeedValue.Id, SeedValue.PatrolRoute);
}

/** User Story: As a systems bots ai consumer, I need to invoke from route seeds through a stable signature so the systems bots ai workflow remains explicit and composable. @fn template <typename Seed> TArray<FBotAIComponent> FromRouteSeeds(const TArray<Seed> &Seeds) */
template <typename Seed>
TArray<FBotAIComponent> FromRouteSeeds(const TArray<Seed> &Seeds) {
  return BotSourceMappingAdapters::MapSeedComponents<Seed, FBotAISource,
                                             FBotAIComponent>(
      Seeds, RouteAISource<Seed>, Component);
}

} // namespace

/** User Story: As a systems bots ai consumer, I need to invoke create initial state through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIState CreateInitialState() */
FBotAIState CreateInitialState() {
  FBotAIState State;
  State.Items = BotAIAdapters::BotAIAdapter().getInitialState();
  return State;
}

/** User Story: As a systems bots ai consumer, I need to invoke component through a stable signature so the systems bots ai workflow remains explicit and composable. @fn FBotAIComponent Component(const FBotAISource &Source) */
FBotAIComponent Component(const FBotAISource &Source) {
  FBotAIComponent Result;
  Result.Id = Source.Id;
  Result.BehaviorState = Source.BehaviorState;
  Result.TargetEntityId = Source.TargetEntityId;
  Result.TargetLocation = Source.TargetLocation;
  Result.bHasTargetLocation = Source.bHasTargetLocation;
  Result.PatrolIndex = Source.PatrolIndex;
  Result.PatrolRoute = Source.PatrolRoute;
  return Result;
}

/** User Story: As a systems bots ai consumer, I need to invoke from townspeople through a stable signature so the systems bots ai workflow remains explicit and composable. @fn TArray<FBotAIComponent> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) */
TArray<FBotAIComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return FromRouteSeeds<FTownspersonSeed>(Seeds);
}

/** User Story: As a systems bots ai consumer, I need to invoke from horses through a stable signature so the systems bots ai workflow remains explicit and composable. @fn TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) */
TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return FromRouteSeeds<FHorseRouteSeed>(Seeds);
}

} // namespace BotAIFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
