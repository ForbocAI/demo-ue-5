#include "Features/Systems/Bots/Orchestrator/OrchestratorAdapters.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Core/frmt.hpp"
#include "Core/fp.hpp"
#include "Features/Systems/Bots/Orchestrator/OrchestratorThunks.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/ForbocAI/Protocol/NPC/NPCThunks.h"
#include "Features/Systems/ForbocAI/Protocol/ProtocolSelectors.h"
#include "Features/Systems/SystemsSelectors.h"

using namespace ForbocAI::Game::Level;

namespace {

/** User Story: As a systems bots orchestrator consumer, I need to invoke runtime state through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn const ForbocAI::Game::Level::FRuntimeState &RuntimeState() */
const ForbocAI::Game::Level::FRuntimeState &RuntimeState() {
  return RuntimeSelectors::SelectState();
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot settings through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FBotSettings BotSettings() */
FBotSettings BotSettings() {
  return RuntimeSelectors::SelectBotSettings(RuntimeState());
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot initial local point through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FLevelLocalPoint BotInitialLocalPoint(const FBotSettings &Settings) */
FLevelLocalPoint BotInitialLocalPoint(const FBotSettings &Settings) {
  return {static_cast<float>(Settings.Spawn.InitialPosition.X),
          static_cast<float>(Settings.Spawn.InitialPosition.Y),
          static_cast<float>(Settings.Spawn.InitialPosition.Z)};
}

void LogNpcCommandFailure(AActor *Actor, const FString &Error,
                          const FBotSettings &Settings) {
  const FString ActorName =
      Actor ? Actor->GetName() : Settings.Diagnostics.NullActorLabel;
  const FString ProcessFailedLog = frmt::RuntimeString(
      Settings.Diagnostics.ProcessFailedLogFormat,
      frmt::Args({frmt::Arg(ActorName), frmt::Arg(Error)}));
  UE_LOG(LogTemp, Warning,
         TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C),
         *ProcessFailedLog);
}

} // namespace

/** User Story: As a systems bots orchestrator consumer, I need to invoke abot orchestrator adapter through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn ABotOrchestratorAdapter::ABotOrchestratorAdapter() */
ABotOrchestratorAdapter::ABotOrchestratorAdapter() {
  const FBotSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings().Bot;
  PrimaryActorTick.bCanEverTick =
      Settings.Lifecycle.bOrchestratorCanEverTick;
  ObservationInterval = Settings.Schedule.ObservationIntervalSeconds;
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke begin play through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void ABotOrchestratorAdapter::BeginPlay() */
void ABotOrchestratorAdapter::BeginPlay() {
  Super::BeginPlay();
  const FBotSettings Settings = BotSettings();
  ObservationInterval = Settings.Schedule.ObservationIntervalSeconds;
  UE_LOG(LogTemp, Display, TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C), *Settings.Diagnostics.StartLog);
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
                Settings.Schedule.ObservationIntervalSeconds)
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
    func::ematch(
        ForbocAINpcThunks::CreateNpc(Persona, RuntimeState()),
        [Actor, Settings](const FString &Error) {
          LogNpcCommandFailure(Actor, Error, Settings);
        },
        [this, Actor, Persona, BotId, InitialLocalPoint, Settings](
            const ProtocolCLI::FCommandExecution &Created) {
          FBotRuntimeBinding Binding;
          Binding.Id = BotId;
          Binding.BotActor = Actor;
          Binding.LastObservationTime =
              Settings.Schedule.InitialObservationTimeSeconds;
          Binding.NpcId = Created.Output;
          Binding.Persona = Persona;
          BotBindings.Add(Actor, Binding);
          DispatchRuntimeActionsForRegistration(
              FBotRegistrationDispatchSource{
                  FBotRegistrationPayloadSource{BotId, Persona, Settings},
                  FBotPositionPayloadSource{BotId, InitialLocalPoint,
                                            Actor->GetActorLocation(),
                                            Settings}});
          const FString RegisteredLog = frmt::RuntimeString(
              Settings.Diagnostics.RegisteredLogFormat,
              frmt::Args({frmt::Arg(Actor->GetName())}));
          UE_LOG(LogTemp, Display,
                 TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C),
                 *RegisteredLog);
        });
  }();
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke request next action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void ABotOrchestratorAdapter::RequestNextAction( const FBotRuntimeBinding &Binding) */
void ABotOrchestratorAdapter::RequestNextAction(
    const FBotRuntimeBinding &Binding) {
  return Binding.NpcId.IsEmpty()
             ? void()
             : [&]() {
    const FBotSettings Settings = BotSettings();
    const FString Observation = GetStateObservation(Binding.Id);
    AActor *BotActor = Binding.BotActor;
    const FRuntimeState &State = RuntimeState();
    func::ematch(
        ForbocAINpcThunks::ProcessNpc(Binding.NpcId, State)(Observation),
        [BotActor, Settings](const FString &Error) {
          LogNpcCommandFailure(BotActor, Error, Settings);
        },
        [this, BotActor, Settings, &State](
            const ProtocolCLI::FCommandExecution &Processed) {
          const auto &ProtocolSettings = State.ForbocAIProtocol.Settings;
          func::match(
              ForbocAIProtocolSelectors::SelectNpcAction(
                  Processed.Output,
                  ProtocolSettings.Npc.Presentation.ActionLinePrefix),
              [this, BotActor](const FString &Action) {
                ExecuteAction(BotActor, Action);
              },
              [BotActor, Settings, &ProtocolSettings]() {
                LogNpcCommandFailure(BotActor,
                                     ProtocolSettings.Errors.ActionMissing,
                                     Settings);
              });
        });
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
        Settings.Diagnostics.ExecuteLogFormat,
        frmt::Args(
            {frmt::Arg(ActionType),
             frmt::Arg(BotActor->GetName())}));
    UE_LOG(LogTemp, Display, TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C), *ExecuteLog);

    RunBotActionDispatch(FBotActionDispatchRequest{
        ActionType, FBotActionDispatchContext{Binding->Id, BotActor, InitialLocalPoint, Settings}});
  }();
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke get state observation through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FString ABotOrchestratorAdapter::GetStateObservation( const FString &BotId) const */
FString ABotOrchestratorAdapter::GetStateObservation(
    const FString &BotId) const {
  const ForbocAI::Game::Level::FRuntimeState &State = RuntimeState();
  return BotStateObservation(FBotObservationSource{
      BotId, RuntimeSelectors::SelectBotSettings(State),
      RuntimeSelectors::SelectBotById(State, BotId),
      RuntimeSelectors::SelectBotPositionById(State, BotId),
      RuntimeSelectors::SelectBotAIById(State, BotId)});
}
