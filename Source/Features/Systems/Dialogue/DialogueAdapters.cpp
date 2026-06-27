
#include "Features/Systems/Dialogue/DialogueAdapters.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "NPC/NPCModule.h"
#endif

UDialogueComponent::UDialogueComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueComponent::InitializeDialogue() {
#if WITH_FORBOC_AI_SDK_DEMO
  FAgentConfig Config;
  Config.Persona = Persona;

  Agent = MakeShared<const FAgent>(AgentFactory::Create(Config));

  UE_LOG(LogTemp, Display,
         TEXT("DialogueComponent: Initialized agent '%s' with persona '%s'"),
         *Agent->Id, *Persona);

  OnDialogueReady(Agent->Id);
#else
  const FString LocalAgentId =
      FString::Printf(TEXT("offline-%s"), *Persona.Replace(TEXT(" "), TEXT("-")));

  UE_LOG(LogTemp, Display,
         TEXT("DialogueComponent: SDK gate closed; initialized local persona '%s' as '%s'"),
         *Persona, *LocalAgentId);

  OnDialogueReady(LocalAgentId);
#endif
}

void UDialogueComponent::SendDialogue(const FString &PlayerInput) {
#if WITH_FORBOC_AI_SDK_DEMO
  return !Agent.IsValid()
             ? (OnDialogueError(
                    TEXT("Cannot send dialogue: agent not initialized")),
                void())
             : [&]() {
    // Record player input in history
    AppendToHistory(TEXT("Player"), PlayerInput);

    UE_LOG(LogTemp, Display, TEXT("DialogueComponent: Player says: '%s'"),
           *PlayerInput);

    // Process via SDK async pipeline
    AgentOps::Process(*Agent, PlayerInput, {})
        .then([this](FAgentResponse Response) {
          // Record NPC response in history
          AppendToHistory(TEXT("NPC"), Response.Dialogue);

          UE_LOG(LogTemp, Display,
                 TEXT("DialogueComponent: NPC responds: '%s'"),
                 *Response.Dialogue);

          DialogueResponse.Broadcast(Response.Dialogue);
          OnDialogueResponse(Response.Dialogue);
        })
        .catch_([this](std::string Error) {
          OnDialogueError(FString(UTF8_TO_TCHAR(Error.c_str())));
        })
        .execute();
  }();
#else
  AppendToHistory(TEXT("Player"), PlayerInput);

  const FString NPCText = FString::Printf(
      TEXT("%s considers the question and answers from the local demo path: \"%s\""),
      *Persona, *PlayerInput);

  AppendToHistory(TEXT("NPC"), NPCText);

  UE_LOG(LogTemp, Display,
         TEXT("DialogueComponent: Offline NPC responds: '%s'"), *NPCText);

  DialogueResponse.Broadcast(NPCText);
  OnDialogueResponse(NPCText);
#endif
}

TArray<FString> UDialogueComponent::GetConversationHistory() const {
  return ConversationHistory;
}

int32 UDialogueComponent::GetExchangeCount() const {
  // Each exchange is a Player + NPC pair = 2 entries
  return ConversationHistory.Num() / 2;
}

void UDialogueComponent::ClearHistory() {
  ConversationHistory.Empty();
  UE_LOG(LogTemp, Display,
         TEXT("DialogueComponent: Conversation history cleared"));
}

void UDialogueComponent::AppendToHistory(const FString &Role,
                                          const FString &Text) {
  ConversationHistory.Add(FString::Printf(TEXT("%s: %s"), *Role, *Text));

  // Trim oldest entries if over max (recursive removal — FP-compliant)
  const auto TrimRecursive = [this](const auto &Self) -> void {
    return ConversationHistory.Num() <= MaxHistorySize
               ? void()
               : (ConversationHistory.RemoveAt(0), Self(Self));
  };
  TrimRecursive(TrimRecursive);
}
