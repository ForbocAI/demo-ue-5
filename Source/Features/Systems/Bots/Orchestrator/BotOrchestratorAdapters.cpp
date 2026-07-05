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

struct FBotRegistrationDispatchSource {
  FBotRegistrationPayloadSource Registration;
  FBotPositionPayloadSource Position;
};

struct FBotActionDispatchContext {
  FString Id;
  AActor *BotActor;
  FLevelLocalPoint InitialLocalPoint;
  FBotRuntimeSettings Settings;
};

using FBotActionDispatch = void (*)(const FBotActionDispatchContext &);

struct FBotActionDispatchDeclaration {
  FString FBotRuntimeSettings::*ActionType;
  FBotActionDispatch Dispatch;
};

void DispatchMoveAction(const FBotActionDispatchContext &Context);
void DispatchAttackAction(const FBotActionDispatchContext &Context);

template <typename Source> struct TBotRuntimeActionDeclaration {
  rtk::AnyAction (*Build)(const Source &);
};

struct FBotTickActionCatalog {};
struct FBotRegistrationActionCatalog {};

template <typename Declaration> struct TBotOrchestratorRegistry;

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

const FRuntimeState &RuntimeState() { return Store::GetStore().getState(); }

FBotRuntimeSettings BotRuntimeSettings() {
  return RuntimeSelectors::SelectBotRuntimeSettings(RuntimeState());
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

rtk::AnyAction PipelineObservedFromPositionSource(
    const FBotPositionPayloadSource &Source) {
  return PipelineObservedAction(Source.Id);
}

rtk::AnyAction BotPositionMovedFromPositionSource(
    const FBotPositionPayloadSource &Source) {
  return BotPositionMovedAction(Source);
}

rtk::AnyAction BotUpsertedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return BotUpsertedAction(Source.Registration);
}

rtk::AnyAction BotPositionUpsertedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return BotPositionUpsertedAction(Source.Position);
}

rtk::AnyAction OrchestratorObservedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return OrchestratorObservedAction(Source.Registration.Id);
}

template <> struct TBotOrchestratorRegistry<FBotTickActionCatalog> {
  static const TArray<TBotRuntimeActionDeclaration<FBotPositionPayloadSource>>
      &Declarations() {
    static const TArray<
        TBotRuntimeActionDeclaration<FBotPositionPayloadSource>>
        RegisteredDeclarations = {{PipelineObservedFromPositionSource},
                                  {BotPositionMovedFromPositionSource}};
    return RegisteredDeclarations;
  }
};

template <>
struct TBotOrchestratorRegistry<FBotRegistrationActionCatalog> {
  static const TArray<
      TBotRuntimeActionDeclaration<FBotRegistrationDispatchSource>>
      &Declarations() {
    static const TArray<
        TBotRuntimeActionDeclaration<FBotRegistrationDispatchSource>>
        RegisteredDeclarations = {
            {BotUpsertedFromRegistrationSource},
            {BotPositionUpsertedFromRegistrationSource},
            {OrchestratorObservedFromRegistrationSource}};
    return RegisteredDeclarations;
  }
};

template <> struct TBotOrchestratorRegistry<FBotActionDispatchDeclaration> {
  static const TArray<FBotActionDispatchDeclaration> &Declarations() {
    static const TArray<FBotActionDispatchDeclaration>
        RegisteredDeclarations = {
            {&FBotRuntimeSettings::MoveActionType, DispatchMoveAction},
            {&FBotRuntimeSettings::AttackActionType, DispatchAttackAction}};
    return RegisteredDeclarations;
  }
};

void DispatchRuntimeAction(const rtk::AnyAction &Action) {
  Store::GetStore().dispatch(Action);
}

template <typename Source>
TArray<rtk::AnyAction> RuntimeActions(
    const Source &SourceValue,
    const TArray<TBotRuntimeActionDeclaration<Source>> &Declarations) {
  return func::map_array<TBotRuntimeActionDeclaration<Source>,
                         rtk::AnyAction>(
      Declarations,
      [&SourceValue](
          const TBotRuntimeActionDeclaration<Source> &Declaration) {
        return Declaration.Build(SourceValue);
      });
}

void DispatchRuntimeActions(const TArray<rtk::AnyAction> &Actions) {
  func::for_each_array<rtk::AnyAction>(Actions, DispatchRuntimeAction);
}

template <typename Source>
void DispatchRuntimeActions(
    const Source &SourceValue,
    const TArray<TBotRuntimeActionDeclaration<Source>> &Declarations) {
  DispatchRuntimeActions(RuntimeActions<Source>(SourceValue, Declarations));
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
    const FBotActionDispatchRequest &Request) {
  const TArray<FBotActionDispatchDeclaration> &Declarations =
      TBotOrchestratorRegistry<
          FBotActionDispatchDeclaration>::Declarations();
  return func::fold_array<FBotActionDispatchDeclaration, bool>(
      Declarations, false,
      [&Request](bool bHandled,
                 const FBotActionDispatchDeclaration &Declaration) {
        return bHandled ||
                       (Request.Context.Settings.*(Declaration.ActionType)) !=
                           Request.ActionType
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
            ? (DispatchRuntimeActions<FBotPositionPayloadSource>(
                   {Binding->Id, InitialLocalPoint,
                    Binding->BotActor->GetActorLocation(), Settings},
                   TBotOrchestratorRegistry<
                       FBotTickActionCatalog>::Declarations()),
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
    DispatchRuntimeActions<FBotRegistrationDispatchSource>(
        {{BotId, Persona, Settings},
         {BotId, InitialLocalPoint, Actor->GetActorLocation(), Settings}},
        TBotOrchestratorRegistry<
            FBotRegistrationActionCatalog>::Declarations());

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
        {ActionType, {Binding->Id, BotActor, InitialLocalPoint, Settings}});
  }();
}

FString ABotOrchestratorAdapter::GetStateObservation(
    const FString &BotId) const {
  const FRuntimeState &State = RuntimeState();
  return BotStateObservation(
      {BotId, RuntimeSelectors::SelectBotRuntimeSettings(State),
       RuntimeSelectors::SelectBotById(State, BotId),
       RuntimeSelectors::SelectBotPositionById(State, BotId),
       RuntimeSelectors::SelectBotAIById(State, BotId)});
}
