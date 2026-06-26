#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WalkingHorse.generated.h"

class USkeletalMeshComponent;
class UTextRenderComponent;

USTRUCT(BlueprintType)
struct FWalkingHorseConfig {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Horse")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Horse")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Horse")
  bool bMountedRider = false;
};

UCLASS()
class DEMOPROJECT_API AWalkingHorse : public AActor {
  GENERATED_BODY()

public:
  AWalkingHorse();

  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "Map|Horse")
  void ConfigureHorse(const FWalkingHorseConfig &Config);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Horse")
  FString HorseName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Horse")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Horse")
  float WalkSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Horse")
  float PauseDuration;

private:
  UPROPERTY(VisibleAnywhere, Category = "Map|Horse")
  USceneComponent *SceneRoot;

  UPROPERTY(VisibleAnywhere, Category = "Map|Horse")
  USkeletalMeshComponent *ImportedHorseMesh;

  UPROPERTY(VisibleAnywhere, Category = "Map|Horse")
  USkeletalMeshComponent *MountedRiderMesh;

  UPROPERTY(VisibleAnywhere, Category = "Map|Horse")
  UTextRenderComponent *NameText;

  int32 PatrolIndex;
  float PauseRemaining;
  bool bMountedRider;

  void AdvancePatrol(float DeltaTime);
  void ConfigureImportedHorseAsset();
  void RefreshText();
};
