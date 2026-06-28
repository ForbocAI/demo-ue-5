#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TownspersonView.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;

USTRUCT(BlueprintType)
struct FTownspersonViewConfig {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Id;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Role;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Persona;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString InteractionPrompt;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString DefaultPlayerLine;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString PinnedResponse;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  TArray<FVector> PatrolRoute;
};

UCLASS()
class DEMOPROJECT_API ATownspersonView : public AActor {
  GENERATED_BODY()

public:
  ATownspersonView();

  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "Level|NPC")
  void ConfigureTownsperson(const FTownspersonViewConfig &Config);

  UFUNCTION(BlueprintCallable, Category = "Level|NPC")
  void ShowDialogueReply(const FString &Reply);

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  bool IsPlayerNearby() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetTownspersonId() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetTownspersonName() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetRole() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetPersona() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetDefaultPlayerLine() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetPinnedResponse() const;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString TownspersonId;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString TownspersonName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString TownspersonRole;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Persona;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString InteractionPrompt;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString DefaultPlayerLine;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString PinnedResponse;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  float WalkSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  float PauseDuration;

private:
  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  USceneComponent *SceneRoot;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  USkeletalMeshComponent *CharacterMesh;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  USphereComponent *InteractionSphere;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  UTextRenderComponent *NameText;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  UTextRenderComponent *PromptText;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  UTextRenderComponent *DialogueText;

  int32 PatrolIndex;
  float PauseRemaining;
  float PatrolArrivalDistance;
  bool bPlayerNearby;

  void AdvancePatrol(float DeltaTime);
  void ConfigureSampleCharacterAsset();
  void RefreshText();

  UFUNCTION()
  void HandleInteractionBegin(UPrimitiveComponent *OverlappedComponent,
                              AActor *OtherActor,
                              UPrimitiveComponent *OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult &SweepResult);

  UFUNCTION()
  void HandleInteractionEnd(UPrimitiveComponent *OverlappedComponent,
                            AActor *OtherActor,
                            UPrimitiveComponent *OtherComp,
                            int32 OtherBodyIndex);
};
