
#pragma once

#include "AgentModule.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "DialogueComponent.generated.h"

/**
 * UDialogueComponent — Actor component for handling incoming Speak actions
 * from the ForbocAI SDK.
 *
 * This is a UE boundary layer: UActorComponent requires UCLASS and virtual
 * overrides. The component delegates all logic to SDK-side free functions.
 *
 * Responsibilities:
 *   - Receives dialogue text from AgentOps::Process responses
 *   - Maintains an immutable conversation history (append-only)
 *   - Fires Blueprint events for UI consumption (UMG, HUD, etc.)
 *   - Provides a functional interface for dialogue state queries
 *
 * User Story: As a game developer, I need a reusable dialogue component so
 * any actor can participate in SDK-driven conversations without custom wiring.
 */
UCLASS(ClassGroup = (ForbocAI), meta = (BlueprintSpawnableComponent))
class DEMOPROJECT_API UDialogueComponent : public UActorComponent {
  GENERATED_BODY()

public:
  UDialogueComponent();

  // --- Configuration ---

  /** The persona identity for this NPC's dialogue. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Dialogue")
  FString Persona = TEXT("NPC");

  /** API URL for the SDK backend. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Dialogue")
  FString ApiUrl = TEXT("http://localhost:8080");

  /** Maximum conversation history entries to retain. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Dialogue")
  int32 MaxHistorySize = 50;

  // --- State ---

  /** Current agent — immutable reference via TSharedPtr. */
  TSharedPtr<const FAgent> Agent;

  // --- Blueprint Callable ---

  /** Initialize the dialogue agent from configured Persona/ApiUrl. */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI|Dialogue")
  void InitializeDialogue();

  /** Send player input and receive NPC response via callback event. */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI|Dialogue")
  void SendDialogue(const FString &PlayerInput);

  /** Get the conversation history as an array of strings. */
  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ForbocAI|Dialogue")
  TArray<FString> GetConversationHistory() const;

  /** Get the number of exchanges in the conversation. */
  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "ForbocAI|Dialogue")
  int32 GetExchangeCount() const;

  /** Clear the conversation history. */
  UFUNCTION(BlueprintCallable, Category = "ForbocAI|Dialogue")
  void ClearHistory();

  // --- Blueprint Events ---

  /** Fired when the NPC responds to dialogue. */
  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI|Dialogue")
  void OnDialogueResponse(const FString &NPCText);

  /** Fired when the dialogue agent is ready. */
  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI|Dialogue")
  void OnDialogueReady(const FString &AgentId);

  /** Fired when a dialogue error occurs. */
  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI|Dialogue")
  void OnDialogueError(const FString &ErrorMessage);

private:
  /**
   * Conversation history — append-only array of "Role: Text" entries.
   * Not a UPROPERTY because we manage it functionally.
   */
  TArray<FString> ConversationHistory;

  /** Append an entry to history (returns new history, updates in place for UE compat). */
  void AppendToHistory(const FString &Role, const FString &Text);
};
