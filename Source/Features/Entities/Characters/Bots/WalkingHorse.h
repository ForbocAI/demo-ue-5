#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WalkingHorse.generated.h"

class USkeletalMeshComponent;
class UTextRenderComponent;

USTRUCT(BlueprintType)
struct FWalkingHorseConfig {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  bool bMountedRider = false;
};

UCLASS()
class DEMOPROJECT_API AWalkingHorse : public AActor {
  GENERATED_BODY()

public:
  AWalkingHorse();

  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "Level|Horse")
  void ConfigureHorse(const FWalkingHorseConfig &Config);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  FString HorseName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  float WalkSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  float PauseDuration;

private:
  UPROPERTY(VisibleAnywhere, Category = "Level|Horse")
  USceneComponent *SceneRoot;

  UPROPERTY(VisibleAnywhere, Category = "Level|Horse")
  USkeletalMeshComponent *ImportedHorseMesh;

  UPROPERTY(VisibleAnywhere, Category = "Level|Horse")
  USkeletalMeshComponent *MountedRiderMesh;

  UPROPERTY(VisibleAnywhere, Category = "Level|Horse")
  UTextRenderComponent *NameText;

  int32 PatrolIndex;
  float PauseRemaining;
  bool bMountedRider;

  void AdvancePatrol(float DeltaTime);
  void ConfigureImportedHorseAsset();
  void RefreshText();
};
