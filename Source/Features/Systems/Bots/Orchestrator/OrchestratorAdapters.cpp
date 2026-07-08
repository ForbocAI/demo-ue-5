#include "Features/Systems/Bots/Orchestrator/OrchestratorAdapters.h"

#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Features/Systems/Bots/Orchestrator/OrchestratorThunks.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/SystemsSelectors.h"

using namespace ForbocAI::Game::Level;

namespace {

const FRuntimeState &RuntimeState() { return RuntimeSelectors::SelectState(); }

FBotSettings BotSettings() {
  return RuntimeSelectors::SelectBotSettings(RuntimeState());
}

FLevelLocalPoint BotInitialLocalPoint(const FBotSettings &Settings) {
  return {static_cast<float>(Settings.InitialPosition.X),
          static_cast<float>(Settings.InitialPosition.Y),
          static_cast<float>(Settings.InitialPosition.Z)};
}

} // namespace

ABotOrchestratorAdapter::ABotOrchestratorAdapter() {
  PrimaryActorTick.bCanEverTick =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings()
          .Bot.bOrchestratorCanEverTick;
}

void ABotOrchestratorAdapter::BeginPlay() {
  Super::BeginPlay();
  const FBotSettings Settings = BotSettings();
  ObservationInterval = Settings.ObservationIntervalSeconds;
  UE_LOG(LogTemp, Display, TEXT("%s"), *Settings.StartLog);
}

void ABotOrchestratorAdapter::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  const FBotSettings Settings = BotSettings();
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
            ? (DispatchRuntimeActionsForTick(
                   FBotPositionPayloadSource{Binding->Id, InitialLocalPoint,
                    Binding->BotActor->GetActorLocation(), Settings}),
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
    const FBotSettings Settings = BotSettings();
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
    DispatchRuntimeActionsForRegistration(
        FBotRegistrationDispatchSource{
            FBotRegistrationPayloadSource{BotId, Persona, Settings},
            FBotPositionPayloadSource{BotId, InitialLocalPoint, Actor->GetActorLocation(), Settings}
        });

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
    const FBotSettings Settings = BotSettings();
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
    const FBotSettings Settings = BotSettings();
    const FLevelLocalPoint InitialLocalPoint = BotInitialLocalPoint(Settings);
    const FString ExecuteLog = frmt::RuntimeString(
        Settings.ExecuteLogFormat,
        frmt::Args(
            {frmt::Arg(ActionType),
             frmt::Arg(BotActor->GetName())}));
    UE_LOG(LogTemp, Display, TEXT("%s"), *ExecuteLog);

    RunBotActionDispatch(FBotActionDispatchRequest{
        ActionType, FBotActionDispatchContext{Binding->Id, BotActor, InitialLocalPoint, Settings}});
  }();
}

FString ABotOrchestratorAdapter::GetStateObservation(
    const FString &BotId) const {
  const FRuntimeState &State = RuntimeState();
  return BotStateObservation(FBotObservationSource{
      BotId, RuntimeSelectors::SelectBotSettings(State),
      RuntimeSelectors::SelectBotById(State, BotId),
      RuntimeSelectors::SelectBotPositionById(State, BotId),
      RuntimeSelectors::SelectBotAIById(State, BotId)});
}
