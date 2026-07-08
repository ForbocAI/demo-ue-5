#include "Features/Systems/Bots/AI/Slice.h"

#include "Features/Systems/Bots/AI/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIReducers {

FBotAIState ReduceBotAISeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FBotAIComponent>> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  Next.Items =
      BotAIAdapters::BotAIAdapter().setAll(State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

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

FBotAIState ReduceTownspeopleSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotAIState Next) -> FBotAIState {
  Next.Items = BotAIAdapters::BotAIAdapter().upsertMany(
      State.Items, BotAIFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

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

#include "Features/Systems/Bots/Horses/Actions.h"
#include "Features/Systems/Bots/Townspeople/Actions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAISlice {

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
#include "Features/Systems/Bots/Source/Mapping/Adapters.h"
#include "Features/Systems/Bots/AI/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIFactories {
namespace {

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

template <typename Seed> FBotAISource RouteAISource(const Seed &SeedValue) {
  return PatrolAISource(SeedValue.Id, SeedValue.PatrolRoute);
}

template <typename Seed>
TArray<FBotAIComponent> FromRouteSeeds(const TArray<Seed> &Seeds) {
  return BotSourceMappingAdapters::MapSeedComponents<Seed, FBotAISource,
                                             FBotAIComponent>(
      Seeds, RouteAISource<Seed>, Component);
}

} // namespace

FBotAIState CreateInitialState() {
  FBotAIState State;
  State.Items = BotAIAdapters::BotAIAdapter().getInitialState();
  return State;
}

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

TArray<FBotAIComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return FromRouteSeeds<FTownspersonSeed>(Seeds);
}

TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return FromRouteSeeds<FHorseRouteSeed>(Seeds);
}

} // namespace BotAIFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
