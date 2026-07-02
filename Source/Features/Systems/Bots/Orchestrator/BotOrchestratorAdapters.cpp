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

#include <initializer_list>

using namespace ForbocAI::Demo::Level;

namespace {

using FBotRuntimeSettings = ForbocAI::Demo::Data::FBotRuntimeSettings;

struct FBotPositionPayloadSource {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  FBotRuntimeSettings Settings;
};

struct FBotRegistrationPayloadSource {
  FString Id;
  FString Persona;
  FBotRuntimeSettings Settings;
};

struct FBotActionDispatchContext {
  FString Id;
  AActor *BotActor;
  FLevelLocalPoint InitialLocalPoint;
  FBotRuntimeSettings Settings;
};

using FBotActionDispatch = void (*)(const FBotActionDispatchContext &);

struct FBotActionDispatchDeclaration {
  FString ActionType;
  FBotActionDispatch Dispatch;

  FBotActionDispatchDeclaration(const FString &InActionType,
                                FBotActionDispatch InDispatch)
      : ActionType(InActionType), Dispatch(InDispatch) {}
};

struct FBotActionDispatchRequest {
  FString ActionType;
  FBotActionDispatchContext Context;
};

struct FBotObservationSource {
  FString BotId;
  FBotRuntimeSettings Settings;
  func::Maybe<FBotEntity> Bot;
  func::Maybe<FBotPositionComponent> Position;
  func::Maybe<FBotAIComponent> AI;
};

FBotRuntimeSettings BotRuntimeSettings() {
  return RuntimeSelectors::SelectBotRuntimeSettings(
      Store::GetStore().getState());
}

FLevelLocalPoint BotInitialLocalPoint(const FBotRuntimeSettings &Settings) {
  return {static_cast<float>(Settings.InitialPosition.X),
          static_cast<float>(Settings.InitialPosition.Y),
          static_cast<float>(Settings.InitialPosition.Z)};
}

FString BotDisplayName(const FString &BotId, const FString &Persona) {
  return Persona.IsEmpty() ? BotId : Persona;
}

FBotEntity BotEntityPayload(const FBotRegistrationPayloadSource &Source) {
  return {Source.Id, BotDisplayName(Source.Id, Source.Persona),
          EBotEntityKind::Townsperson, EBotAlignment::Friendly,
          Source.Settings.bRegisteredBotActive};
}

FBotPositionMoved
BotPositionMovedPayload(const FBotPositionPayloadSource &Source) {
  return {Source.Id, Source.LocalLocation, Source.WorldLocation,
          Source.Settings.bPositionPayloadHasLocalLocation,
          Source.Settings.bPositionPayloadHasWorldLocation};
}

FBotPositionComponent
BotPositionComponentPayload(const FBotPositionPayloadSource &Source) {
  return {Source.Id, Source.LocalLocation, Source.WorldLocation,
          Source.Settings.bPositionPayloadHasLocalLocation,
          Source.Settings.bPositionPayloadHasWorldLocation};
}

FBotPipelinePayload BotPipelinePayload(const FString &BotId) {
  return {BotId};
}

FBotOrchestratorPayload BotOrchestratorPayload(const FString &BotId) {
  return {BotId};
}

rtk::AnyAction BotUpsertedAction(
    const FBotRegistrationPayloadSource &Source) {
  return BotActions::BotUpserted()(BotEntityPayload(Source));
}

rtk::AnyAction BotPositionMovedAction(
    const FBotPositionPayloadSource &Source) {
  return BotPositionActions::BotPositionMoved()(
      BotPositionMovedPayload(Source));
}

rtk::AnyAction BotPositionUpsertedAction(
    const FBotPositionPayloadSource &Source) {
  return BotPositionActions::BotPositionUpserted()(
      BotPositionComponentPayload(Source));
}

rtk::AnyAction PipelineObservedAction(const FString &BotId) {
  return BotPipelineActions::PipelineObserved()(BotPipelinePayload(BotId));
}

rtk::AnyAction OrchestratorObservedAction(const FString &BotId) {
  return BotOrchestratorActions::OrchestratorObserved()(
      BotOrchestratorPayload(BotId));
}

void DispatchRuntimeAction(const rtk::AnyAction &Action) {
  Store::GetStore().dispatch(Action);
}

void DispatchRuntimeActions(std::initializer_list<rtk::AnyAction> Actions) {
  func::for_each_array<rtk::AnyAction>(
      TArray<rtk::AnyAction>(Actions), DispatchRuntimeAction);
}

void DispatchMoveAction(const FBotActionDispatchContext &Context) {
  DispatchRuntimeAction(BotPositionMovedAction(
      {Context.Id, Context.InitialLocalPoint,
       Context.BotActor->GetActorLocation() + Context.Settings.MoveActionOffset,
       Context.Settings}));
}

void DispatchAttackAction(const FBotActionDispatchContext &Context) {
  DispatchRuntimeAction(OrchestratorObservedAction(Context.Id));
}

bool RunBotActionDispatch(
    const FBotActionDispatchRequest &Request,
    std::initializer_list<FBotActionDispatchDeclaration> Declarations) {
  const TArray<FBotActionDispatchDeclaration> DeclarationList(Declarations);
  return func::fold_array<FBotActionDispatchDeclaration, bool>(
      DeclarationList, false,
      [&Request](bool bHandled,
                 const FBotActionDispatchDeclaration &Declaration) {
        return bHandled || Declaration.ActionType != Request.ActionType
                   ? bHandled
                   : (Declaration.Dispatch(Request.Context), true);
      });
}

FString ObservationDisplayName(const FBotObservationSource &Source) {
  return Source.Bot.hasValue ? Source.Bot.value.DisplayName : Source.BotId;
}

FVector ObservationWorldLocation(const FBotObservationSource &Source) {
  return Source.Position.hasValue && Source.Position.value.bHasWorldLocation
             ? Source.Position.value.WorldLocation
             : Source.Settings.InitialPosition;
}

int32 ObservationBehaviorState(const FBotObservationSource &Source) {
  return Source.AI.hasValue
             ? static_cast<int32>(Source.AI.value.BehaviorState)
             : Source.Settings.DefaultBehaviorState;
}

FString BotStateObservation(const FBotObservationSource &Source) {
  return frmt::RuntimeString(
      Source.Settings.StateObservationFormat,
      frmt::Args(
          {frmt::Arg(Source.BotId),
           frmt::Arg(ObservationDisplayName(Source)),
           frmt::Arg(ObservationWorldLocation(Source).ToString()),
           frmt::Arg(ObservationBehaviorState(Source))}));
}

} // namespace

ABotOrchestratorAdapter::ABotOrchestratorAdapter() {
  PrimaryActorTick.bCanEverTick =
      ForbocAI::Demo::Data::RuntimeSettingsAdapters::LoadDemoRuntimeSettings()
          .BotRuntime.bOrchestratorCanEverTick;
}

void ABotOrchestratorAdapter::BeginPlay() {
  Super::BeginPlay();
  const FBotRuntimeSettings Settings = BotRuntimeSettings();
  ObservationInterval = Settings.ObservationIntervalSeconds;
  UE_LOG(LogTemp, Display, TEXT("%s"), *Settings.StartLog);
}

void ABotOrchestratorAdapter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  const FBotRuntimeSettings Settings = BotRuntimeSettings();
  const FLevelLocalPoint InitialLocalPoint = BotInitialLocalPoint(Settings);
  const float CurrentTime = GetWorld()->GetTimeSeconds();
  TArray<AActor *> Keys;
  BotBindings.GetKeys(Keys);

  func::for_each_array<AActor *>(
      Keys,
      [this, CurrentTime, InitialLocalPoint,
       Settings](AActor *const &BotKey) {
        FBotRuntimeBinding *Binding = BotBindings.Find(BotKey);
        Binding
            ? (DispatchRuntimeActions(
                   {PipelineObservedAction(Binding->Id),
                    BotPositionMovedAction(
                        {Binding->Id, InitialLocalPoint,
                         Binding->BotActor->GetActorLocation(), Settings})}),
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
    const FBotRuntimeSettings Settings = BotRuntimeSettings();
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
    DispatchRuntimeActions(
        {BotUpsertedAction({BotId, Persona, Settings}),
         BotPositionUpsertedAction(
             {BotId, InitialLocalPoint, Actor->GetActorLocation(), Settings}),
         OrchestratorObservedAction(BotId)});

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
    const FBotRuntimeSettings Settings = BotRuntimeSettings();
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
    const FBotRuntimeSettings Settings = BotRuntimeSettings();
    const FLevelLocalPoint InitialLocalPoint = BotInitialLocalPoint(Settings);
    const FString ExecuteLog = frmt::RuntimeString(
        Settings.ExecuteLogFormat,
        frmt::Args(
            {frmt::Arg(ActionType),
             frmt::Arg(BotActor->GetName())}));
    UE_LOG(LogTemp, Display, TEXT("%s"), *ExecuteLog);

    RunBotActionDispatch(
        {ActionType, {Binding->Id, BotActor, InitialLocalPoint, Settings}},
        {{Settings.MoveActionType, DispatchMoveAction},
         {Settings.AttackActionType, DispatchAttackAction}});
  }();
}

FString ABotOrchestratorAdapter::GetStateObservation(
    const FString &BotId) const {
  const FRuntimeState &State = Store::GetStore().getState();
  return BotStateObservation(
      {BotId, RuntimeSelectors::SelectBotRuntimeSettings(State),
       RuntimeSelectors::SelectBotById(State, BotId),
       RuntimeSelectors::SelectBotPositionById(State, BotId),
       RuntimeSelectors::SelectBotAIById(State, BotId)});
}
