
#include "Dialogue/DialogueComponent.h"
#include "NPC/NPCModule.h"

UDialogueComponent::UDialogueComponent() {
  PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueComponent::InitializeDialogue() {
  FAgentConfig Config;
  Config.Persona = Persona;

  Agent = MakeShared<const FAgent>(AgentFactory::Create(Config));

  UE_LOG(LogTemp, Display,
         TEXT("DialogueComponent: Initialized agent '%s' with persona '%s'"),
         *Agent->Id, *Persona);

  OnDialogueReady(Agent->Id);
}

void UDialogueComponent::SendDialogue(const FString &PlayerInput) {
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

          // Fire Blueprint event for UI consumption
          OnDialogueResponse(Response.Dialogue);
        })
        .catch_([this](std::string Error) {
          OnDialogueError(FString(UTF8_TO_TCHAR(Error.c_str())));
        })
        .execute();
  }();
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
