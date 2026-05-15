#include "BotOrchestrator.h"
#include "State/Actions.h"

using namespace ForbocAI::Orchestrator;

ABotOrchestrator::ABotOrchestrator() {
  PrimaryActorTick.bCanEverTick = true;
  RegistryStore = Factory::CreateOrchestratorStore();
}

void ABotOrchestrator::BeginPlay() {
  Super::BeginPlay();
  UE_LOG(LogTemp, Display, TEXT("BotOrchestrator: Brain Online."));
}

void ABotOrchestrator::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  float CurrentTime = GetWorld()->GetTimeSeconds();

  // Get immutable snapshot of the registry
  FRegistryState Registry = RegistryStore.GetState();
  TArray<AActor *> Keys;
  Registry.Bots.GetKeys(Keys);

  // Recursive tick dispatch — FP-compliant (no for-loops)
  const auto TickBotsRecursive =
      [this, DeltaTime, CurrentTime, &Registry](const TArray<AActor *> &BotKeys,
                                                 int32 Idx,
                                                 const auto &Self) -> void {
    return Idx >= BotKeys.Num()
               ? void()
               : ([&]() {
                    FBotInstance Instance = Registry.Bots[BotKeys[Idx]];

                    // Functional Store Tick (Heartbeat)
                    ForbocAI::State::FActionTick TickAction;
                    TickAction.DeltaTime = DeltaTime;
                    Instance.Store.Dispatch(TickAction);

                    // Observation Logic (Interval-based)
                    (CurrentTime - Instance.LastObservationTime >=
                     ObservationInterval)
                        ? (Instance.LastObservationTime = CurrentTime,
                           RequestNextAction(Instance),
                           // Dispatch update back to store
                           RegistryStore.Dispatch(
                               FRegistryAction(TInPlaceType<FUpdateInstanceAction>(),
                                               FUpdateInstanceAction{BotKeys[Idx],
                                                                     Instance})),
                           void())
                        : void();
                  }(),
                  Self(BotKeys, Idx + 1, Self));
  };
  TickBotsRecursive(Keys, 0, TickBotsRecursive);
}

void ABotOrchestrator::RegisterBot(AActor *Actor, FString Persona) {
  return !Actor
             ? void()
             : [&]() {
    FBotInstance Instance;
    Instance.BotActor = Actor;

    // Initialize Functional Store
    Instance.Store =
        ForbocAI::Bot::Factory::CreateBotStore(Actor->GetName());

    // Initialize SDK Agent
    FAgentConfig Config;
    Config.Persona = Persona;

    // AgentFactory::Create returns FAgent directly (immutable value).
    Instance.Agent =
        MakeShared<const FAgent>(AgentFactory::Create(Config));

    // Dispatch registration action to the registry store
    RegistryStore.Dispatch(
        FRegistryAction(TInPlaceType<FRegisterBotAction>(),
                        FRegisterBotAction{Actor, Instance}));

    UE_LOG(LogTemp, Display,
           TEXT("BotOrchestrator: Registered Bot '%s'"),
           *Actor->GetName());
  }();
}

void ABotOrchestrator::RequestNextAction(FBotInstance &Instance) {
  return !Instance.Agent.IsValid()
             ? void()
             : [&]() {
    // Step 1: OBSERVE
    ForbocAI::State::FBotState InternalState = Instance.Store.GetState();
    FString Observation = GetStateObservation(InternalState);

    // Step 2-6: Protocol Pipeline (Directive -> Generate -> Verdict)
    AActor *BotActor = Instance.BotActor;

    AgentOps::Process(*Instance.Agent, Observation, {})
        .then([this, BotActor](FAgentResponse Response) {
          // Step 7: EXECUTE
          ExecuteAction(BotActor, Response.Action);
        })
        .catch_([BotActor](std::string Error) {
          UE_LOG(LogTemp, Warning,
                 TEXT("BotOrchestrator: Process failed for %s: %s"),
                 BotActor ? *BotActor->GetName() : TEXT("<null>"),
                 *FString(UTF8_TO_TCHAR(Error.c_str())));
        })
        .execute();
  }();
}

void ABotOrchestrator::ExecuteAction(AActor *BotActor,
                                     const FAgentAction &Action) {
  FRegistryState Registry = RegistryStore.GetState();

  return (!BotActor || !Registry.Bots.Contains(BotActor))
             ? void()
             : [&]() {
    FBotInstance Instance = Registry.Bots[BotActor];

    UE_LOG(LogTemp, Display, TEXT("BotOrchestrator: Executing '%s' for %s"),
           *Action.Type, *BotActor->GetName());

    // Map SDK Action -> Functional Action -> Dispatch to Store
    (Action.Type == TEXT("MOVE"))
        ? ([&]() {
            ForbocAI::State::FActionMove Move;
            Move.TargetLocation =
                BotActor->GetActorLocation() + FVector(500, 0, 0);
            Move.Speed = 100.0f;
            Instance.Store.Dispatch(Move);
          }(),
          void())
    : (Action.Type == TEXT("ATTACK"))
        ? ([&]() {
            ForbocAI::State::FActionAttack Attack;
            Instance.Store.Dispatch(Attack);
          }(),
          void())
        : void(); // Unhandled action type — no-op

    // Dispatch updated instance back to registry
    RegistryStore.Dispatch(
        FRegistryAction(TInPlaceType<FUpdateInstanceAction>(),
                        FUpdateInstanceAction{BotActor, Instance}));
  }();
}

FString
ABotOrchestrator::GetStateObservation(const ForbocAI::State::FBotState &State) {
  return FString::Printf(
      TEXT("Name: %s, Health: %.1f, Position: %s, Phase: %d"), *State.Name,
      State.Stats.Health, *State.Position.ToString(), (int32)State.Phase);
}
