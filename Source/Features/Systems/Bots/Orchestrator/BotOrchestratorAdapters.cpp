#include "Features/Systems/Bots/Orchestrator/BotOrchestratorAdapters.h"

#include "Core/ecs.hpp"
#include "Features/Systems/Bots/BotActions.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorActions.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineActions.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Store.h"

using namespace ForbocAI::Demo::Level;

ABotOrchestratorAdapter::ABotOrchestratorAdapter() {
  PrimaryActorTick.bCanEverTick = true;
}

void ABotOrchestratorAdapter::BeginPlay() {
  Super::BeginPlay();
  UE_LOG(LogTemp, Display, TEXT("BotOrchestrator: Brain Online."));
}

void ABotOrchestratorAdapter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  const float CurrentTime = GetWorld()->GetTimeSeconds();
  TArray<AActor *> Keys;
  BotBindings.GetKeys(Keys);

  ecs::forEachArray<AActor *>(
      Keys, [this, DeltaTime, CurrentTime](AActor *const &BotKey) {
        FBotRuntimeBinding *Binding = BotBindings.Find(BotKey);
        Binding
            ? (Store::GetStore().dispatch(
                   BotPipelineActions::PipelineObserved()(
                       FBotPipelinePayload{Binding->Id})),
               Store::GetStore().dispatch(
                   BotPositionActions::BotPositionMoved()(
                       FBotPositionMoved{
                           Binding->Id, FLevelLocalPoint{0.0f, 0.0f, 0.0f},
                           Binding->BotActor->GetActorLocation(), true,
                           true})),
               (CurrentTime - Binding->LastObservationTime >=
                ObservationInterval)
                   ? (Binding->LastObservationTime = CurrentTime,
                      RequestNextAction(*Binding), void())
                   : void(),
               void())
            : void();
      });
}

void ABotOrchestratorAdapter::RegisterBot(AActor *Actor, FString Persona) {
  return !Actor
             ? void()
             : [&]() {
    const FString BotId = Actor->GetName();
    FBotRuntimeBinding Binding;
    Binding.Id = BotId;
    Binding.BotActor = Actor;

#if WITH_FORBOC_AI_SDK_DEMO
    FAgentConfig Config;
    Config.Persona = Persona;
    Binding.Agent = MakeShared<const FAgent>(AgentFactory::Create(Config));
#else
    UE_LOG(LogTemp, Display,
           TEXT("BotOrchestrator: SDK gate closed; registering '%s' with local state only"),
           *Persona);
#endif

    BotBindings.Add(Actor, Binding);
    Store::GetStore().dispatch(BotActions::BotUpserted()(
        FBotEntity{BotId, Persona.IsEmpty() ? BotId : Persona,
                   EBotEntityKind::Townsperson, EBotAlignment::Friendly,
                   true}));
    Store::GetStore().dispatch(BotPositionActions::BotPositionUpserted()(
        FBotPositionComponent{BotId, FLevelLocalPoint{0.0f, 0.0f, 0.0f},
                              Actor->GetActorLocation(), true, true}));
    Store::GetStore().dispatch(BotOrchestratorActions::OrchestratorObserved()(
        FBotOrchestratorPayload{BotId}));

    UE_LOG(LogTemp, Display,
           TEXT("BotOrchestrator: Registered Bot '%s'"),
           *Actor->GetName());
  }();
}

void ABotOrchestratorAdapter::RequestNextAction(
    const FBotRuntimeBinding &Binding) {
#if WITH_FORBOC_AI_SDK_DEMO
  return !Binding.Agent.IsValid()
             ? void()
             : [&]() {
    const FString Observation = GetStateObservation(Binding.Id);

    AActor *BotActor = Binding.BotActor;

    AgentOps::Process(*Binding.Agent, Observation, {})
        .then([this, BotActor](FAgentResponse Response) {
          ExecuteAction(BotActor, Response.Action.Type);
        })
        .catch_([BotActor](std::string Error) {
          UE_LOG(LogTemp, Warning,
                 TEXT("BotOrchestrator: Process failed for %s: %s"),
                 BotActor ? *BotActor->GetName() : TEXT("<null>"),
                 *FString(UTF8_TO_TCHAR(Error.c_str())));
        })
        .execute();
  }();
#else
  const FString ActionType =
      RuntimeSelectors::SelectBotAIById(Store::GetStore().getState(),
                                        Binding.Id)
              .hasValue
          ? TEXT("MOVE")
          : TEXT("MOVE");

  UE_LOG(LogTemp, Display,
         TEXT("BotOrchestrator: SDK gate closed; local observation '%s' -> %s"),
         *GetStateObservation(Binding.Id), *ActionType);

  ExecuteAction(Binding.BotActor, ActionType);
#endif
}

void ABotOrchestratorAdapter::ExecuteAction(AActor *BotActor,
                                     const FString &ActionType) {
  FBotRuntimeBinding *Binding = BotActor ? BotBindings.Find(BotActor) : nullptr;

  return !Binding
             ? void()
             : [&]() {
    UE_LOG(LogTemp, Display, TEXT("BotOrchestrator: Executing '%s' for %s"),
           *ActionType, *BotActor->GetName());

    (ActionType == TEXT("MOVE"))
        ? ([&]() {
            Store::GetStore().dispatch(BotPositionActions::BotPositionMoved()(
                FBotPositionMoved{Binding->Id,
                                  FLevelLocalPoint{0.0f, 0.0f, 0.0f},
                                  BotActor->GetActorLocation() +
                                      FVector(500, 0, 0),
                                  true,
                                  true}));
          }(),
          void())
    : (ActionType == TEXT("ATTACK"))
        ? ([&]() {
            Store::GetStore().dispatch(
                BotOrchestratorActions::OrchestratorObserved()(
                    FBotOrchestratorPayload{Binding->Id}));
          }(),
          void())
        : void();
  }();
}

FString ABotOrchestratorAdapter::GetStateObservation(
    const FString &BotId) const {
  const FRuntimeState &State = Store::GetStore().getState();
  const func::Maybe<FBotEntity> Bot = RuntimeSelectors::SelectBotById(State, BotId);
  const func::Maybe<FBotPositionComponent> Position =
      RuntimeSelectors::SelectBotPositionById(State, BotId);
  const func::Maybe<FBotAIComponent> AI =
      RuntimeSelectors::SelectBotAIById(State, BotId);
  const FString DisplayName = Bot.hasValue ? Bot.value.DisplayName : BotId;
  const FVector WorldLocation =
      Position.hasValue && Position.value.bHasWorldLocation
          ? Position.value.WorldLocation
          : FVector::ZeroVector;
  const int32 BehaviorState =
      AI.hasValue ? static_cast<int32>(AI.value.BehaviorState) : 0;
  return FString::Printf(
      TEXT("Id: %s, Name: %s, Position: %s, BehaviorState: %d"), *BotId,
      *DisplayName, *WorldLocation.ToString(), BehaviorState);
}
