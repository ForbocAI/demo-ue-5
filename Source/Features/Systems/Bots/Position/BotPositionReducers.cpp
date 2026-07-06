#include "Features/Systems/Bots/Position/BotPositionReducers.h"

#include "Features/Systems/Bots/Position/BotPositionAdapters.h"
#include "Features/Systems/Bots/Position/BotPositionFactories.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionReducers {
namespace {

FBotPatrolAdvancePayload
ReduceIdlePatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload;
  Payload.Location = Request.CurrentLocation;
  Payload.PatrolIndex = Request.PatrolIndex;
  Payload.PauseRemaining = Request.PauseRemaining;
  return Payload;
}

FBotPatrolAdvancePayload
ReducePauseCountdownPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload = ReduceIdlePatrolAdvance(Request);
  Payload.PauseRemaining =
      FMath::Max(0.0f, Request.PauseRemaining - Request.DeltaTime);
  return Payload;
}

FBotPatrolAdvancePayload
ReduceArrivedPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload = ReduceIdlePatrolAdvance(Request);
  Payload.PatrolIndex = (Request.PatrolIndex + 1) % Request.PatrolRoute.Num();
  Payload.PauseRemaining = Request.PauseDuration;
  return Payload;
}

FBotPatrolAdvancePayload
ReduceMovingPatrolAdvance(const FBotPatrolAdvanceRequest &Request,
                          const FBotPatrolMovementDelta &Movement) {
  FBotPatrolAdvancePayload Payload = ReduceIdlePatrolAdvance(Request);
  Payload.Location =
      Request.CurrentLocation +
      Movement.Delta.GetSafeNormal() *
          FMath::Min(Movement.Distance, Request.WalkSpeed * Request.DeltaTime);
  Payload.Rotation = Movement.Delta.Rotation();
  Payload.bShouldMove = true;
  Payload.bShouldRotate = true;
  return Payload;
}

} // namespace

FBotPositionState ReduceBotPositionsSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FBotPositionComponent>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().setAll(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

FBotPositionState ReduceBotPositionUpserted(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionComponent> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertOne(
      State.Items, Action.PayloadValue);
  return Next;
  }).val;
}

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

FBotPositionState ReduceTownspeopleSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertMany(
      State.Items, BotPositionFactories::FromTownspeople(Action.PayloadValue));
  return Next;
  }).val;
}

FBotPositionState ReduceHorsesSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) {
  return (func::pipe(State) | [&](FBotPositionState Next) -> FBotPositionState {
  Next.Items = BotPositionAdapters::BotPositionAdapter().upsertMany(
      State.Items, BotPositionFactories::FromHorses(Action.PayloadValue));
  return Next;
  }).val;
}

FBotPositionState ReduceInitialPatrolObserved(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotInitialPatrolLocationRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FBotPositionState Next) -> FBotPositionState {
            Next.LastInitialPatrolIndex =
                ReduceInitialPatrolIndex(Action.PayloadValue.PatrolRoute);
            Next.LastInitialPatrolLocation =
                ReduceInitialPatrolLocation(Action.PayloadValue);
            return Next;
          })
      .val;
}

FBotPositionState ReducePatrolAdvanceObserved(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPatrolAdvanceRequest> &Action) {
  return (func::pipe(State) |
          [&Action](FBotPositionState Next) -> FBotPositionState {
            Next.LastPatrolAdvance = ReducePatrolAdvance(Action.PayloadValue);
            return Next;
          })
      .val;
}

int32 ReduceInitialPatrolIndex(const TArray<FVector> &PatrolRoute) {
  return PatrolRoute.Num() > 1 ? 1 : 0;
}

FBotInitialPatrolLocationPayload ReduceInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request) {
  FBotInitialPatrolLocationPayload Payload;
  Payload.bShouldMove = Request.PatrolRoute.Num() > 0;
  Payload.Location =
      Payload.bShouldMove ? Request.PatrolRoute[0] : FVector::ZeroVector;
  return Payload;
}

FBotPatrolAdvancePayload
ReducePatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  return Request.PatrolRoute.Num() < 2
             ? ReduceIdlePatrolAdvance(Request)
             : Request.PauseRemaining > 0.0f
                   ? ReducePauseCountdownPatrolAdvance(Request)
                   : [&]() {
                       const FVector Target =
                           Request.PatrolRoute[Request.PatrolIndex];
                       const FVector Delta = Target - Request.CurrentLocation;
                       const float Distance = Delta.Size();
                       const FBotPatrolMovementDelta Movement{Delta, Distance};
                       return Distance < Request.ArrivalDistance
                                  ? ReduceArrivedPatrolAdvance(Request)
                                  : ReduceMovingPatrolAdvance(Request,
                                                              Movement);
                     }();
}

} // namespace BotPositionReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
