#include "Features/Systems/Bots/Position/PositionSlice.h"

#include "Features/Systems/Bots/Position/PositionAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionReducers {
namespace {

/** User Story: As a systems bots position consumer, I need to invoke reduce idle patrol advance through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPatrolAdvancePayload ReduceIdlePatrolAdvance(const FBotPatrolAdvanceRequest &Request) */
FBotPatrolAdvancePayload
ReduceIdlePatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload;
  Payload.Location = Request.Pose.CurrentLocation;
  Payload.PatrolIndex = Request.Progress.PatrolIndex;
  Payload.PauseRemaining = Request.Progress.PauseRemaining;
  return Payload;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce pause countdown patrol advance through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPatrolAdvancePayload ReducePauseCountdownPatrolAdvance(const FBotPatrolAdvanceRequest &Request) */
FBotPatrolAdvancePayload
ReducePauseCountdownPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload = ReduceIdlePatrolAdvance(Request);
  Payload.PauseRemaining =
      FMath::Max(0.0f, Request.Progress.PauseRemaining -
                           Request.Timing.DeltaTime);
  return Payload;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce arrived patrol advance through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPatrolAdvancePayload ReduceArrivedPatrolAdvance(const FBotPatrolAdvanceRequest &Request) */
FBotPatrolAdvancePayload
ReduceArrivedPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload = ReduceIdlePatrolAdvance(Request);
  Payload.PatrolIndex =
      (Request.Progress.PatrolIndex + 1) % Request.PatrolRoute.Num();
  Payload.PauseRemaining = Request.Timing.PauseDuration;
  return Payload;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce moving patrol advance through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPatrolAdvancePayload ReduceMovingPatrolAdvance(const FBotPatrolAdvanceRequest &Request, const FBotPatrolMovementDelta &Movement) */
FBotPatrolAdvancePayload
ReduceMovingPatrolAdvance(const FBotPatrolAdvanceRequest &Request,
                          const FBotPatrolMovementDelta &Movement) {
  FBotPatrolAdvancePayload Payload = ReduceIdlePatrolAdvance(Request);
  Payload.Location =
      Request.Pose.CurrentLocation +
      Movement.Delta.GetSafeNormal() *
          FMath::Min(Movement.Distance,
                     Request.Timing.WalkSpeed * Request.Timing.DeltaTime);
  Payload.Rotation = Movement.Delta.Rotation();
  Payload.bShouldMove = true;
  Payload.bShouldRotate = true;
  return Payload;
}

} // namespace

/** User Story: As a systems bots position consumer, I need to invoke reduce bot positions seeded through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceBotPositionsSeeded( const FBotPositionState &State, const rtk::PayloadAction<TArray<FBotPositionComponent>> &Action) */
FBotPositionState ReduceBotPositionsSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FBotPositionComponent>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce bot position upserted through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceBotPositionUpserted( const FBotPositionState &State, const rtk::PayloadAction<FBotPositionComponent> &Action) */
FBotPositionState ReduceBotPositionUpserted(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionComponent> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertOne(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce bot position moved through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceBotPositionMoved( const FBotPositionState &State, const rtk::PayloadAction<FBotPositionMoved> &Action) */
FBotPositionState ReduceBotPositionMoved(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionMoved> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  const FBotPositionMoved Payload = Action.PayloadValue;
  Next.Items = BotPositionAdapters::BotPositionAdapter().updateOne(
      State.Items, Payload.Id,
      [Payload](const FBotPositionComponent &Current)
          -> FBotPositionComponent {
        FBotPositionComponent Updated = Current;
        Updated.LocalLocation = Payload.LocalLocation;
        Updated.WorldLocation = Payload.WorldLocation;
        Updated.bHasWorldLocation = Payload.bHasWorldLocation;
        Updated.bFacingRight = Payload.bFacingRight;
        return Updated;
      });
  return Next;
  }).val;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceTownspeopleSeeded( const FBotPositionState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FBotPositionState ReduceTownspeopleSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertMany(
      State.Items, BotPositionFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceHorsesSeeded( const FBotPositionState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
FBotPositionState ReduceHorsesSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertMany(
      State.Items, BotPositionFactories::FromHorses(Action.PayloadValue));
  return Next;
  }).val;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce initial patrol index through a stable signature so the systems bots position workflow remains explicit and composable. @fn int32 ReduceInitialPatrolIndex(const TArray<FVector> &PatrolRoute) */
int32 ReduceInitialPatrolIndex(const TArray<FVector> &PatrolRoute) {
  return PatrolRoute.Num() > 1 ? 1 : 0;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce initial patrol location through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotInitialPatrolLocationPayload ReduceInitialPatrolLocation( const FBotInitialPatrolLocationRequest &Request) */
FBotInitialPatrolLocationPayload ReduceInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request) {
  FBotInitialPatrolLocationPayload Payload;
  Payload.bShouldMove = Request.PatrolRoute.Num() > 0;
  Payload.Location =
      Payload.bShouldMove ? Request.PatrolRoute[0] : FVector::ZeroVector;
  return Payload;
}

/** User Story: As a systems bots position consumer, I need to invoke reduce patrol advance through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPatrolAdvancePayload ReducePatrolAdvance(const FBotPatrolAdvanceRequest &Request) */
FBotPatrolAdvancePayload
ReducePatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  return Request.PatrolRoute.Num() < 2
             ? ReduceIdlePatrolAdvance(Request)
             : Request.Progress.PauseRemaining > 0.0f
                   ? ReducePauseCountdownPatrolAdvance(Request)
                   : [&]() {
                       const FVector Target =
                           Request.PatrolRoute[Request.Progress.PatrolIndex];
                       const FVector Delta =
                           Target - Request.Pose.CurrentLocation;
                       const float Distance = Delta.Size();
                       const FBotPatrolMovementDelta Movement{Delta, Distance};
                       return Distance < Request.Timing.ArrivalDistance
                                  ? ReduceArrivedPatrolAdvance(Request)
                                  : ReduceMovingPatrolAdvance(Request,
                                                              Movement);
                     }();
}

} // namespace BotPositionReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Horses/HorsesActions.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionSlice {

/** User Story: As a systems bots position consumer, I need to invoke get slice through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::Slice<FBotPositionState> &GetSlice() */
const rtk::Slice<FBotPositionState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FBotPositionState>> Slice =
      func::lazy([]() -> rtk::Slice<FBotPositionState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FBotPositionState>(
          TEXT("botPosition"), BotPositionFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FBotPositionState> &Builder) {
    Builder.addCase(BotPositionActions::BotPositionsSeeded(),
                                BotPositionReducers::ReduceBotPositionsSeeded);
    Builder.addCase(BotPositionActions::BotPositionUpserted(),
                                BotPositionReducers::ReduceBotPositionUpserted);
    Builder.addCase(BotPositionActions::BotPositionMoved(),
                                BotPositionReducers::ReduceBotPositionMoved);
    Builder.addCase(TownspersonActions::TownspeopleSeeded(),
                                BotPositionReducers::ReduceTownspeopleSeeded);
    Builder.addCase(HorseActions::HorsesSeeded(),
                                BotPositionReducers::ReduceHorsesSeeded);
  });
      });
  return func::eval(Slice);
}

} // namespace BotPositionSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI


#include "Core/ecs.hpp"
#include "Features/Systems/Bots/Source/Mapping/MappingAdapters.h"
#include "Features/Systems/Bots/Position/PositionAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionFactories {
namespace {

/** User Story: As a systems bots position consumer, I need to invoke position source through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionSource PositionSource(const FString &Id, const TArray<FLevelLocalPoint> &PatrolRoute) */
FBotPositionSource PositionSource(const FString &Id,
                                  const TArray<FLevelLocalPoint> &PatrolRoute) {
  return {Id, BotSourceMappingAdapters::FirstRoutePoint(PatrolRoute),
          FVector::ZeroVector, false, true};
}

/** User Story: As a systems bots position consumer, I need to invoke route position source through a stable signature so the systems bots position workflow remains explicit and composable. @fn template <typename Seed> FBotPositionSource RoutePositionSource(const Seed &SeedValue) */
template <typename Seed>
FBotPositionSource RoutePositionSource(const Seed &SeedValue) {
  return PositionSource(SeedValue.Id, SeedValue.PatrolRoute);
}

/** User Story: As a systems bots position consumer, I need to invoke from route seeds through a stable signature so the systems bots position workflow remains explicit and composable. @fn template <typename Seed> TArray<FBotPositionComponent> FromRouteSeeds(const TArray<Seed> &Seeds) */
template <typename Seed>
TArray<FBotPositionComponent> FromRouteSeeds(const TArray<Seed> &Seeds) {
  return BotSourceMappingAdapters::MapSeedComponents<Seed, FBotPositionSource,
                                             FBotPositionComponent>(
      Seeds, RoutePositionSource<Seed>, Component);
}

} // namespace

/** User Story: As a systems bots position consumer, I need to invoke create initial state through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState CreateInitialState() */
FBotPositionState CreateInitialState() {
  FBotPositionState State;
  State.Items = BotPositionAdapters::BotPositionAdapter().getInitialState();
  return State;
}

/** User Story: As a systems bots position consumer, I need to invoke component through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionComponent Component(const FBotPositionSource &Source) */
FBotPositionComponent Component(const FBotPositionSource &Source) {
  FBotPositionComponent Result;
  Result.Id = Source.Id;
  Result.LocalLocation = Source.LocalLocation;
  Result.WorldLocation = Source.WorldLocation;
  Result.bHasWorldLocation = Source.bHasWorldLocation;
  Result.bFacingRight = Source.bFacingRight;
  return Result;
}

/** User Story: As a systems bots position consumer, I need to invoke from townspeople through a stable signature so the systems bots position workflow remains explicit and composable. @fn TArray<FBotPositionComponent> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) */
TArray<FBotPositionComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds) {
  return FromRouteSeeds<FTownspersonSeed>(Seeds);
}

/** User Story: As a systems bots position consumer, I need to invoke from horses through a stable signature so the systems bots position workflow remains explicit and composable. @fn TArray<FBotPositionComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) */
TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds) {
  return FromRouteSeeds<FHorseRouteSeed>(Seeds);
}

} // namespace BotPositionFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
