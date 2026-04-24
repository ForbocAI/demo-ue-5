#include "BotOrchestrator.h"
#include "State/Actions.h"

ABotOrchestrator::ABotOrchestrator() { PrimaryActorTick.bCanEverTick = true; }

void ABotOrchestrator::BeginPlay() {
  Super::BeginPlay();
  UE_LOG(LogTemp, Display, TEXT("BotOrchestrator: Brain Online."));
}

void ABotOrchestrator::Tick(float DeltaTime) {
  Super::Tick(DeltaTime);

  float CurrentTime = GetWorld()->GetTimeSeconds();

  // Recursive tick dispatch — FP-compliant (no for-loops)
  TArray<AActor *> Keys;
  ActiveBots.GetKeys(Keys);

  const auto TickBotsRecursive =
      [this, DeltaTime, CurrentTime](const TArray<AActor *> &BotKeys, int32 Idx,
                                     const auto &Self) -> void {
    return Idx >= BotKeys.Num()
               ? void()
               : ([&]() {
                    FBotInstance &Instance = ActiveBots[BotKeys[Idx]];

                    // Functional Store Tick (Heartbeat)
                    ForbocAI::State::FActionTick TickAction;
                    TickAction.DeltaTime = DeltaTime;
                    Instance.Store.Dispatch(TickAction);

                    // Observation Logic (Interval-based)
                    (CurrentTime - Instance.LastObservationTime >=
                     ObservationInterval)
                        ? (Instance.LastObservationTime = CurrentTime,
                           RequestNextAction(Instance), void())
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
    Config.ApiUrl = ApiUrl;

    auto AgentResult = AgentFactory::Create(Config);
    AgentResult.isRight
        ? (Instance.Agent =
               MakeShared<const FAgent>(AgentResult.right),
           ActiveBots.Add(Actor, Instance),
           UE_LOG(LogTemp, Display,
                  TEXT("BotOrchestrator: Registered Bot '%s'"),
                  *Actor->GetName()),
           void())
        : (UE_LOG(LogTemp, Error,
                  TEXT("BotOrchestrator: Failed to create agent: %s"),
                  *AgentResult.left),
           void());
  }();
}

void ABotOrchestrator::RequestNextAction(FBotInstance &Instance) {
  return !Instance.Agent.IsValid()
             ? void()
             : [&]() {
    // Step 1: OBSERVE
    // Combine internal functional state with physical world state
    ForbocAI::State::FBotState InternalState = Instance.Store.GetState();
    FString Observation = GetStateObservation(InternalState);

    // Step 2-6: Protocol Pipeline (Directive -> Generate -> Verdict)
    // Re-captured for safety in the async lambda
    AActor *BotActor = Instance.BotActor;

    AgentOps::Process(*Instance.Agent, Observation, {},
                      [this, BotActor](FAgentResponse Response) {
                        // Step 7: EXECUTE
                        ExecuteAction(BotActor, Response.Action);
                      });
  }();
}

void ABotOrchestrator::ExecuteAction(AActor *BotActor,
                                     const FAgentAction &Action) {
  return (!BotActor || !ActiveBots.Contains(BotActor))
             ? void()
             : [&]() {
    FBotInstance &Instance = ActiveBots[BotActor];

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
  }();
}

FString
ABotOrchestrator::GetStateObservation(const ForbocAI::State::FBotState &State) {
  return FString::Printf(
      TEXT("Name: %s, Health: %.1f, Position: %s, Phase: %d"), *State.Name,
      State.Stats.Health, *State.Position.ToString(), (int32)State.Phase);
}
