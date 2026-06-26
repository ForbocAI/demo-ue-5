#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TalkableTownsperson.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class DEMOPROJECT_API ATalkableTownsperson : public AActor {
  GENERATED_BODY()

public:
  ATalkableTownsperson();

  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "FrenchGulch|NPC")
  void ConfigureTownsperson(const FString &InName, const FString &InRole,
                            const FString &InPersona,
                            const TArray<FVector> &InPatrolRoute);

  UFUNCTION(BlueprintCallable, Category = "FrenchGulch|NPC")
  FString Interact(const FString &PlayerLine);

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FrenchGulch|NPC")
  bool IsPlayerNearby() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FrenchGulch|NPC")
  FString GetTownspersonName() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "FrenchGulch|NPC")
  FString GetRole() const;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|NPC")
  FString TownspersonName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|NPC")
  FString TownspersonRole;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|NPC")
  FString Persona;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|NPC")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|NPC")
  float WalkSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|NPC")
  float PauseDuration;

private:
  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|NPC")
  USceneComponent *SceneRoot;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|NPC")
  UStaticMeshComponent *BodyMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|NPC")
  UStaticMeshComponent *HatMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|NPC")
  USphereComponent *InteractionSphere;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|NPC")
  UTextRenderComponent *NameText;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|NPC")
  UTextRenderComponent *PromptText;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|NPC")
  UTextRenderComponent *DialogueText;

  int32 PatrolIndex;
  float PauseRemaining;
  bool bPlayerNearby;

  void AdvancePatrol(float DeltaTime);
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
