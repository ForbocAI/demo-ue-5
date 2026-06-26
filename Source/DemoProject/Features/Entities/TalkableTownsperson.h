#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TalkableTownsperson.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;

UCLASS()
class DEMOPROJECT_API ATalkableTownsperson : public AActor {
  GENERATED_BODY()

public:
  ATalkableTownsperson();

  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "Map|NPC")
  void ConfigureTownsperson(const FString &InName, const FString &InRole,
                            const FString &InPersona,
                            const TArray<FVector> &InPatrolRoute);

  UFUNCTION(BlueprintCallable, Category = "Map|NPC")
  FString Interact(const FString &PlayerLine);

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Map|NPC")
  bool IsPlayerNearby() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Map|NPC")
  FString GetTownspersonName() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Map|NPC")
  FString GetRole() const;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|NPC")
  FString TownspersonName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|NPC")
  FString TownspersonRole;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|NPC")
  FString Persona;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|NPC")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|NPC")
  float WalkSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|NPC")
  float PauseDuration;

private:
  UPROPERTY(VisibleAnywhere, Category = "Map|NPC")
  USceneComponent *SceneRoot;

  UPROPERTY(VisibleAnywhere, Category = "Map|NPC")
  USkeletalMeshComponent *CharacterMesh;

  UPROPERTY(VisibleAnywhere, Category = "Map|NPC")
  USphereComponent *InteractionSphere;

  UPROPERTY(VisibleAnywhere, Category = "Map|NPC")
  UTextRenderComponent *NameText;

  UPROPERTY(VisibleAnywhere, Category = "Map|NPC")
  UTextRenderComponent *PromptText;

  UPROPERTY(VisibleAnywhere, Category = "Map|NPC")
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
