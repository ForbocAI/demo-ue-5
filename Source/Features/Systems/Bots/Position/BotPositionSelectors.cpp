#include "Features/Systems/Bots/Position/BotPositionSelectors.h"

#include "Features/Systems/Bots/Position/BotPositionAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionSelectors {
namespace {

FBotPatrolAdvancePayload
CreateIdlePatrolAdvancePayload(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload;
  Payload.Location = Request.CurrentLocation;
  Payload.PatrolIndex = Request.PatrolIndex;
  Payload.PauseRemaining = Request.PauseRemaining;
  return Payload;
}

FBotPatrolAdvancePayload
SelectPauseCountdownPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload = CreateIdlePatrolAdvancePayload(Request);
  Payload.PauseRemaining =
      FMath::Max(0.0f, Request.PauseRemaining - Request.DeltaTime);
  return Payload;
}

FBotPatrolAdvancePayload
SelectArrivedPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  FBotPatrolAdvancePayload Payload = CreateIdlePatrolAdvancePayload(Request);
  Payload.PatrolIndex = (Request.PatrolIndex + 1) % Request.PatrolRoute.Num();
  Payload.PauseRemaining = Request.PauseDuration;
  return Payload;
}

FBotPatrolAdvancePayload
SelectMovingPatrolAdvance(const FBotPatrolAdvanceRequest &Request,
                          const FVector &Delta, float Distance) {
  FBotPatrolAdvancePayload Payload = CreateIdlePatrolAdvancePayload(Request);
  Payload.Location =
      Request.CurrentLocation +
      Delta.GetSafeNormal() *
          FMath::Min(Distance, Request.WalkSpeed * Request.DeltaTime);
  Payload.Rotation = Delta.Rotation();
  Payload.bShouldMove = true;
  Payload.bShouldRotate = true;
  return Payload;
}

} // namespace

TArray<FBotPositionComponent> SelectAll(const FBotPositionState &State) {
  return BotPositionAdapters::BotPositionAdapter().getSelectors().selectAll(
      State.Items);
}

func::Maybe<FBotPositionComponent>
SelectById(const FBotPositionState &State, const FString &Id) {
  return BotPositionAdapters::BotPositionAdapter().getSelectors().selectById(
      State.Items, Id);
}

int32 SelectTotal(const FBotPositionState &State) {
  return BotPositionAdapters::BotPositionAdapter().getSelectors().selectTotal(
      State.Items);
}

int32 SelectInitialPatrolIndex(const TArray<FVector> &PatrolRoute) {
  return PatrolRoute.Num() > 1 ? 1 : 0;
}

FBotInitialPatrolLocationPayload SelectInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request) {
  FBotInitialPatrolLocationPayload Payload;
  Payload.bShouldMove = Request.PatrolRoute.Num() > 0;
  Payload.Location =
      Payload.bShouldMove ? Request.PatrolRoute[0] : FVector::ZeroVector;
  return Payload;
}

FBotPatrolAdvancePayload
SelectPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  if (Request.PatrolRoute.Num() < 2) {
    return CreateIdlePatrolAdvancePayload(Request);
  }

  if (Request.PauseRemaining > 0.0f) {
    return SelectPauseCountdownPatrolAdvance(Request);
  }

  const FVector Target = Request.PatrolRoute[Request.PatrolIndex];
  const FVector Delta = Target - Request.CurrentLocation;
  const float Distance = Delta.Size();
  return Distance < Request.ArrivalDistance
             ? SelectArrivedPatrolAdvance(Request)
             : SelectMovingPatrolAdvance(Request, Delta, Distance);
}

} // namespace BotPositionSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
