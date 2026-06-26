#include "Features/Systems/SDK/SDKTestActor.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "Features/Systems/Bots/Orchestrator/Factories/BotFactory.h"
#include "Bridge/BridgeModule.h"
#include "Memory/MemoryModule.h"
#include "NPC/NPCModule.h"
#include "Soul/SoulModule.h"
#endif

ASDKTestActor::ASDKTestActor() {
  PrimaryActorTick.bCanEverTick = false;
  Persona = TEXT("Cyber-Merchant");
}

void ASDKTestActor::BeginPlay() {
  Super::BeginPlay();

  if (Persona.Len() > 0) {
    InitializeAgent();

#if WITH_FORBOC_AI_SDK_DEMO
    if (CurrentAgent.IsValid()) {
      UE_LOG(LogTemp, Display, TEXT("SDKTestActor: Auto-initialized agent %s"),
             *CurrentAgent->Id);
    }
#else
    UE_LOG(LogTemp, Display,
           TEXT("SDKTestActor: SDK gate closed; sample actor is inert."));
#endif
  }
}

void ASDKTestActor::InitializeAgent() {
#if WITH_FORBOC_AI_SDK_DEMO
  FAgentConfig Config;
  Config.Persona = Persona;

  CurrentAgent = MakeShared<const FAgent>(AgentFactory::Create(Config));
  ActiveRules = BridgeOps::CreateRPGRules();

  UE_LOG(LogTemp, Display, TEXT("ForbocAI: Created Agent with Persona '%s'"),
         *Persona);
  UE_LOG(LogTemp, Display,
         TEXT("ForbocAI: Registered %d validation rules via RPG Preset"),
         ActiveRules.Num());

  const auto RegisterRulesRecursive =
      [](const TArray<FValidationRule> &Rules, int32 Idx,
         const auto &Self) -> void {
    return Idx >= Rules.Num()
               ? void()
               : (BridgeOps::RegisterRule(Rules[Idx], FString()),
                  Self(Rules, Idx + 1, Self));
  };
  RegisterRulesRecursive(ActiveRules, 0, RegisterRulesRecursive);

  OnAgentInitialized(CurrentAgent->Id);

  auto BotStore = ForbocAI::Bot::Factory::CreateBotStore(TEXT("TestBotOps"));

  ForbocAI::State::FBotState InitState = BotStore.GetState();
  UE_LOG(LogTemp, Display,
         TEXT("FunctionalCore: Created Bot '%s' (Health: %.0f)"),
         *InitState.Name, InitState.Stats.Health);

  ForbocAI::State::FActionMove MoveAction;
  MoveAction.TargetLocation = FVector(100, 200, 300);
  MoveAction.Speed = 50.0f;
  BotStore.Dispatch(MoveAction);

  ForbocAI::State::FBotState AfterMove = BotStore.GetState();
  UE_LOG(LogTemp, Display, TEXT("FunctionalCore: Post-Move Position: %s"),
         *AfterMove.Position.ToString());

  ForbocAI::State::FActionTakeDamage DamageAction;
  DamageAction.Amount = 80.0f;
  BotStore.Dispatch(DamageAction);

  ForbocAI::State::FBotState AfterDamage = BotStore.GetState();
  UE_LOG(
      LogTemp, Display,
      TEXT("FunctionalCore: Post-Damage HP: %.0f, Phase: %d (Expected Flee=3)"),
      AfterDamage.Stats.Health, (int32)AfterDamage.Phase);
#else
  UE_LOG(LogTemp, Warning,
         TEXT("SDKTestActor: InitializeAgent skipped because FORBOC_DEMO_WITH_SDK is not set."));
  OnAgentInitialized(TEXT("sdk-gate-closed"));
#endif
}

void ASDKTestActor::ProcessInput(const FString &InputText) {
#if WITH_FORBOC_AI_SDK_DEMO
  if (!CurrentAgent.IsValid()) {
    UE_LOG(LogTemp, Warning,
           TEXT("ForbocAI: Cannot process input, agent not initialized."));
    return;
  }

  AgentOps::Process(*CurrentAgent, InputText, {})
      .then([this](FAgentResponse Response) {
        const FBridgeRuleContext ValContext =
            BridgeFactory::CreateContext(&CurrentAgent->State, {});

        const FValidationResult ValResult =
            BridgeOps::Validate(Response.Action, ActiveRules, ValContext);

        if (ValResult.bValid) {
          UE_LOG(LogTemp, Display, TEXT("Bridge: Action VALID (%s)"),
                 *ValResult.Reason);
        } else {
          UE_LOG(LogTemp, Warning, TEXT("Bridge: Action BLOCKED (%s)"),
                 *ValResult.Reason);
        }

        UE_LOG(LogTemp, Display, TEXT("ForbocAI Response: %s"),
               *Response.Dialogue);
        OnAgentResponse(Response.Dialogue);
      })
      .catch_([](std::string Error) {
        UE_LOG(LogTemp, Warning, TEXT("ForbocAI: ProcessInput failed: %s"),
               *FString(UTF8_TO_TCHAR(Error.c_str())));
      })
      .execute();
#else
  const FString Message =
      FString::Printf(TEXT("SDK gate is closed; local actor received: %s"),
                      *InputText);
  UE_LOG(LogTemp, Warning, TEXT("SDKTestActor: %s"), *Message);
  OnAgentResponse(Message);
#endif
}

void ASDKTestActor::UpdateAgentState(const FString &NewStateDescription) {
#if WITH_FORBOC_AI_SDK_DEMO
  if (!CurrentAgent.IsValid()) {
    return;
  }

  const FString JsonState =
      FString::Printf(TEXT("{\"description\": \"%s\"}"), *NewStateDescription);

  const FAgentState NewState = TypeFactory::AgentState(JsonState);
  CurrentAgent =
      MakeShared<const FAgent>(AgentOps::WithState(*CurrentAgent, NewState));

  UE_LOG(LogTemp, Display, TEXT("ForbocAI: Updated Agent State to '%s'"),
         *JsonState);
#else
  UE_LOG(LogTemp, Warning,
         TEXT("SDKTestActor: UpdateAgentState skipped while SDK gate is closed: %s"),
         *NewStateDescription);
#endif
}

void ASDKTestActor::ExportSoul() {
#if WITH_FORBOC_AI_SDK_DEMO
  if (!CurrentAgent.IsValid()) {
    UE_LOG(LogTemp, Warning,
           TEXT("ForbocAI: Cannot export Soul, agent not initialized."));
    return;
  }

  const SoulTypes::SoulCreationResult SoulResult =
      SoulOps::FromAgent(CurrentAgent->State, {}, CurrentAgent->Id,
                         CurrentAgent->Persona);
  if (SoulResult.isLeft) {
    UE_LOG(LogTemp, Warning, TEXT("ForbocAI: Failed to create Soul: %s"),
           *SoulResult.left);
    return;
  }

  const FSoul Soul = SoulResult.right;
  UE_LOG(LogTemp, Display, TEXT("ForbocAI: Exporting Soul to Arweave..."));

  SoulOps::ExportToArweave(Soul, FString())
      .then([this](FSoulExportResult Result) {
        if (Result.TxId.IsEmpty()) {
          UE_LOG(LogTemp, Warning,
                 TEXT("ForbocAI: Soul export returned no transaction id."));
          return;
        }

        UE_LOG(LogTemp, Display, TEXT("ForbocAI: Soul Exported! TxId: %s"),
               *Result.TxId);
        OnSoulExported(Result.TxId);
      })
      .catch_([](std::string Error) {
        UE_LOG(LogTemp, Warning, TEXT("ForbocAI: Soul export rejected: %s"),
               UTF8_TO_TCHAR(Error.c_str()));
      })
      .execute();
#else
  UE_LOG(LogTemp, Warning,
         TEXT("SDKTestActor: ExportSoul skipped because SDK gate is closed."));
#endif
}
