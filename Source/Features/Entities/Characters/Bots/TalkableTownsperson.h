#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TalkableTownsperson.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;

USTRUCT(BlueprintType)
struct FTownspersonConfig {
  GENERATED_BODY()

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
class DEMOPROJECT_API ATalkableTownsperson : public AActor {
  GENERATED_BODY()

public:
  ATalkableTownsperson();

  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "Level|NPC")
  void ConfigureTownsperson(const FTownspersonConfig &Config);

  UFUNCTION(BlueprintCallable, Category = "Level|NPC")
  FString Interact(const FString &PlayerLine);

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  bool IsPlayerNearby() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetTownspersonName() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetRole() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetDefaultPlayerLine() const;

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
