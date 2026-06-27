
#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "NPC/NPCModule.h"
#endif

#include "DialogueAdapters.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueResponseDelegate,
                                            const FString &, NPCText);

/**
 * UDialogueComponent - Actor component for handling incoming Speak actions.
 *
 * This is a UE boundary layer: UActorComponent requires UCLASS and virtual
 * overrides. With the SDK gate open, the component delegates to SDK-side free
 * functions. With the gate closed, it returns deterministic local replies so
 * the demo map remains playable while SDK/API work is pending.
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

  /** Maximum conversation history entries to retain. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI|Dialogue")
  int32 MaxHistorySize = 50;

  // --- State ---

#if WITH_FORBOC_AI_SDK_DEMO
  /** Current agent - immutable reference via TSharedPtr. */
  TSharedPtr<const FAgent> Agent;
#endif

  // --- Blueprint Callable ---

  /** Initialize the dialogue agent from configured Persona. */
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

  /** Bindable response stream for UMG and Blueprint consumers. */
  UPROPERTY(BlueprintAssignable, Category = "ForbocAI|Dialogue")
  FDialogueResponseDelegate DialogueResponse;

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
