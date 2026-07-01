#include "Features/Systems/Bots/Orchestrator/BotOrchestratorAdapters.h"

#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Features/Systems/Bots/BotActions.h"
#include "Features/Systems/Bots/Orchestrator/BotOrchestratorActions.h"
#include "Features/Systems/Bots/Pipeline/BotPipelineActions.h"
#include "Features/Systems/Bots/Position/BotPositionActions.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Store.h"

using namespace ForbocAI::Demo::Level;

namespace {

ForbocAI::Demo::Data::FBotRuntimeSettings BotRuntimeSettings() {
  return RuntimeSelectors::SelectBotRuntimeSettings(
      Store::GetStore().getState());
}

FLevelLocalPoint BotInitialLocalPoint(
    const ForbocAI::Demo::Data::FBotRuntimeSettings &Settings) {
  return {static_cast<float>(Settings.InitialPosition.X),
          static_cast<float>(Settings.InitialPosition.Y),
          static_cast<float>(Settings.InitialPosition.Z)};
}

} // namespace

ABotOrchestratorAdapter::ABotOrchestratorAdapter() {
  PrimaryActorTick.bCanEverTick =
      ForbocAI::Demo::Data::RuntimeSettingsAdapters::LoadDemoRuntimeSettings()
          .BotRuntime.bOrchestratorCanEverTick;
}

void ABotOrchestratorAdapter::BeginPlay() {
  Super::BeginPlay();
  const ForbocAI::Demo::Data::FBotRuntimeSettings Settings =
      BotRuntimeSettings();
  ObservationInterval = Settings.ObservationIntervalSeconds;
  UE_LOG(LogTemp, Display, TEXT("%s"), *Settings.StartLog);
}

void ABotOrchestratorAdapter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  const ForbocAI::Demo::Data::FBotRuntimeSettings Settings =
      BotRuntimeSettings();
  const FLevelLocalPoint InitialLocalPoint = BotInitialLocalPoint(Settings);
  const float CurrentTime = GetWorld()->GetTimeSeconds();
  TArray<AActor *> Keys;
  BotBindings.GetKeys(Keys);

  func::for_each_indexed(
      Keys, static_cast<size_t>(Keys.Num()),
      [this, CurrentTime, InitialLocalPoint,
       Settings](AActor *const &BotKey) {
        FBotRuntimeBinding *Binding = BotBindings.Find(BotKey);
        Binding
            ? (Store::GetStore().dispatch(
                   BotPipelineActions::PipelineObserved()(
                       FBotPipelinePayload{Binding->Id})),
               Store::GetStore().dispatch(
                   BotPositionActions::BotPositionMoved()(
                       FBotPositionMoved{
                           Binding->Id, InitialLocalPoint,
                           Binding->BotActor->GetActorLocation(),
                           Settings.bPositionPayloadHasLocalLocation,
                           Settings.bPositionPayloadHasWorldLocation})),
               (CurrentTime - Binding->LastObservationTime >=
                Settings.ObservationIntervalSeconds)
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
    const ForbocAI::Demo::Data::FBotRuntimeSettings Settings =
        BotRuntimeSettings();
    const FLevelLocalPoint InitialLocalPoint = BotInitialLocalPoint(Settings);
    const FString BotId = Actor->GetName();
    FBotRuntimeBinding Binding;
    Binding.Id = BotId;
    Binding.BotActor = Actor;
    Binding.LastObservationTime = Settings.InitialObservationTimeSeconds;

    FAgentConfig Config;
    Config.Persona = Persona;
    Binding.Agent = MakeShared<const FAgent>(AgentFactory::Create(Config));

    BotBindings.Add(Actor, Binding);
    Store::GetStore().dispatch(BotActions::BotUpserted()(
        FBotEntity{BotId, Persona.IsEmpty() ? BotId : Persona,
                   EBotEntityKind::Townsperson, EBotAlignment::Friendly,
                   Settings.bRegisteredBotActive}));
    Store::GetStore().dispatch(BotPositionActions::BotPositionUpserted()(
        FBotPositionComponent{BotId, InitialLocalPoint,
                              Actor->GetActorLocation(),
                              Settings.bPositionPayloadHasLocalLocation,
                              Settings.bPositionPayloadHasWorldLocation}));
    Store::GetStore().dispatch(BotOrchestratorActions::OrchestratorObserved()(
        FBotOrchestratorPayload{BotId}));

    const FString RegisteredLog =
        frmt::RuntimeString(
            Settings.RegisteredLogFormat,
            frmt::Args(
                {frmt::Arg(Actor->GetName())}));
    UE_LOG(LogTemp, Display, TEXT("%s"), *RegisteredLog);
  }();
}

void ABotOrchestratorAdapter::RequestNextAction(
    const FBotRuntimeBinding &Binding) {
  return !Binding.Agent.IsValid()
             ? void()
             : [&]() {
    const ForbocAI::Demo::Data::FBotRuntimeSettings Settings =
        BotRuntimeSettings();
    const FString Observation = GetStateObservation(Binding.Id);

    AActor *BotActor = Binding.BotActor;

    AgentOps::Process(*Binding.Agent, Observation, {})
        .then([this, BotActor](FAgentResponse Response) {
          ExecuteAction(BotActor, Response.Action.Type);
        })
        .catch_([BotActor, Settings](std::string Error) {
          const FString ActorName =
              BotActor ? BotActor->GetName() : Settings.NullActorLabel;
          const FString ProcessFailedLog =
              frmt::RuntimeString(
                  Settings.ProcessFailedLogFormat,
                  frmt::Args(
                      {frmt::Arg(ActorName),
                       frmt::Arg(
                           FString(UTF8_TO_TCHAR(Error.c_str())))}));
          UE_LOG(LogTemp, Warning, TEXT("%s"), *ProcessFailedLog);
        })
        .execute();
  }();
}

void ABotOrchestratorAdapter::ExecuteAction(AActor *BotActor,
                                     const FString &ActionType) {
  FBotRuntimeBinding *Binding = BotActor ? BotBindings.Find(BotActor) : nullptr;

  return !Binding
             ? void()
             : [&]() {
    const ForbocAI::Demo::Data::FBotRuntimeSettings Settings =
        BotRuntimeSettings();
    const FLevelLocalPoint InitialLocalPoint = BotInitialLocalPoint(Settings);
    const FString ExecuteLog = frmt::RuntimeString(
        Settings.ExecuteLogFormat,
        frmt::Args(
            {frmt::Arg(ActionType),
             frmt::Arg(BotActor->GetName())}));
    UE_LOG(LogTemp, Display, TEXT("%s"), *ExecuteLog);

    (ActionType == Settings.MoveActionType)
        ? ([&]() {
            Store::GetStore().dispatch(BotPositionActions::BotPositionMoved()(
                FBotPositionMoved{Binding->Id,
                                  InitialLocalPoint,
                                  BotActor->GetActorLocation() +
                                      Settings.MoveActionOffset,
                                  Settings.bPositionPayloadHasLocalLocation,
                                  Settings.bPositionPayloadHasWorldLocation}));
          }(),
          void())
    : (ActionType == Settings.AttackActionType)
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
  const ForbocAI::Demo::Data::FBotRuntimeSettings Settings =
      RuntimeSelectors::SelectBotRuntimeSettings(State);
  const func::Maybe<FBotEntity> Bot = RuntimeSelectors::SelectBotById(State, BotId);
  const func::Maybe<FBotPositionComponent> Position =
      RuntimeSelectors::SelectBotPositionById(State, BotId);
  const func::Maybe<FBotAIComponent> AI =
      RuntimeSelectors::SelectBotAIById(State, BotId);
  const FString DisplayName = Bot.hasValue ? Bot.value.DisplayName : BotId;
  const FVector WorldLocation =
      Position.hasValue && Position.value.bHasWorldLocation
          ? Position.value.WorldLocation
          : Settings.InitialPosition;
  const int32 BehaviorState =
      AI.hasValue ? static_cast<int32>(AI.value.BehaviorState)
                  : Settings.DefaultBehaviorState;
  return frmt::RuntimeString(
      Settings.StateObservationFormat,
      frmt::Args(
          {frmt::Arg(BotId),
           frmt::Arg(DisplayName),
           frmt::Arg(WorldLocation.ToString()),
           frmt::Arg(BehaviorState)}));
}
