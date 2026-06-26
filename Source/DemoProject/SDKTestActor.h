#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#if WITH_FORBOC_AI_SDK_DEMO
#include "Bridge/BridgeModule.h"
#include "NPC/NPCModule.h"
#endif

#include "SDKTestActor.generated.h"

/**
 * Demo Actor - ForbocAI SDK integration example.
 *
 * The reflected class is always present so UnrealHeaderTool can parse it.
 * SDK-only state and behavior are compiled only when
 * FORBOC_DEMO_WITH_SDK=1 opens the feature gate.
 */
UCLASS()
class DEMOPROJECT_API ASDKTestActor : public AActor {
  GENERATED_BODY()

public:
  ASDKTestActor();

protected:
  virtual void BeginPlay() override;

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ForbocAI")
  FString Persona;

#if WITH_FORBOC_AI_SDK_DEMO
  TSharedPtr<const FAgent> CurrentAgent;
  TArray<FValidationRule> ActiveRules;
#endif

  UFUNCTION(BlueprintCallable, Category = "ForbocAI")
  void InitializeAgent();

  UFUNCTION(BlueprintCallable, Category = "ForbocAI")
  void ProcessInput(const FString &InputText);

  UFUNCTION(BlueprintCallable, Category = "ForbocAI")
  void UpdateAgentState(const FString &NewStateDescription);

  UFUNCTION(BlueprintCallable, Category = "ForbocAI")
  void ExportSoul();

  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI")
  void OnAgentResponse(const FString &ResponseText);

  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI")
  void OnAgentInitialized(const FString &AgentId);

  UFUNCTION(BlueprintImplementableEvent, Category = "ForbocAI")
  void OnSoulExported(const FString &TxId);
};
