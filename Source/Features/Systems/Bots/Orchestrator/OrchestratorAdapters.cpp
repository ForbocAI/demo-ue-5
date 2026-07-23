#include "Features/Systems/Bots/Orchestrator/OrchestratorAdapters.h"

#include "Core/frmt.hpp"
#include "Core/fp.hpp"
#include "Features/Systems/Bots/Orchestrator/OrchestratorThunks.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/SystemsSelectors.h"

using namespace ForbocAI::Game::Level;

namespace {

/** User Story: As a systems bots orchestrator consumer, I need to invoke runtime state through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn const FRuntimeState &RuntimeState() */
const FRuntimeState &RuntimeState() { return RuntimeSelectors::SelectState(); }

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot settings through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FBotSettings BotSettings() */
FBotSettings BotSettings() {
  return RuntimeSelectors::SelectBotSettings(RuntimeState());
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot initial local point through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FLevelLocalPoint BotInitialLocalPoint(const FBotSettings &Settings) */
FLevelLocalPoint BotInitialLocalPoint(const FBotSettings &Settings) {
  return {static_cast<float>(Settings.InitialPosition.X),
          static_cast<float>(Settings.InitialPosition.Y),
          static_cast<float>(Settings.InitialPosition.Z)};
}

} // namespace

/** User Story: As a systems bots orchestrator consumer, I need to invoke abot orchestrator adapter through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn ABotOrchestratorAdapter::ABotOrchestratorAdapter() */
ABotOrchestratorAdapter::ABotOrchestratorAdapter() {
  PrimaryActorTick.bCanEverTick =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings()
          .Bot.bOrchestratorCanEverTick;
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke begin play through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void ABotOrchestratorAdapter::BeginPlay() */
void ABotOrchestratorAdapter::BeginPlay() {
  Super::BeginPlay();
  const FBotSettings Settings = BotSettings();
  ObservationInterval = Settings.ObservationIntervalSeconds;
  UE_LOG(LogTemp, Display, TEXT("%s"), *Settings.StartLog);
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke tick through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void ABotOrchestratorAdapter::Tick(float DeltaTime) */
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

/** User Story: As a systems bots orchestrator consumer, I need to invoke register bot through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void ABotOrchestratorAdapter::RegisterBot(AActor *Actor, FString Persona) */
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

/** User Story: As a systems bots orchestrator consumer, I need to invoke request next action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void ABotOrchestratorAdapter::RequestNextAction( const FBotRuntimeBinding &Binding) */
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

/** User Story: As a systems bots orchestrator consumer, I need to invoke execute action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void ABotOrchestratorAdapter::ExecuteAction(AActor *BotActor, const FString &ActionType) */
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

/** User Story: As a systems bots orchestrator consumer, I need to invoke get state observation through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FString ABotOrchestratorAdapter::GetStateObservation( const FString &BotId) const */
FString ABotOrchestratorAdapter::GetStateObservation(
    const FString &BotId) const {
  const FRuntimeState &State = RuntimeState();
  return BotStateObservation(FBotObservationSource{
      BotId, RuntimeSelectors::SelectBotSettings(State),
      RuntimeSelectors::SelectBotById(State, BotId),
      RuntimeSelectors::SelectBotPositionById(State, BotId),
      RuntimeSelectors::SelectBotAIById(State, BotId)});
}
