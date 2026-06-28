#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HorseView.generated.h"

class USkeletalMeshComponent;
class UTextRenderComponent;

/**
 * @brief View payload for configuring one horse actor from runtime store data.
 *
 * Architecture: Runtime reducers prepare horse spawn presentation data; this
 * struct is an Unreal-facing adapter shape and does not own store semantics.
 */
USTRUCT(BlueprintType)
struct FHorseViewConfig {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|Horse")
  bool bMountedRider = false;
};

/**
 * @brief Display actor for a reducer-owned horse route payload.
 *
 * Architecture: The actor applies configured presentation and patrol movement.
 * Route and mounted-rider facts originate in feature reducers/thunks, keeping
 * ECS/store data below the view layer.
 *
 * User story: As a player exploring the demo town, horses appear from the same
 * runtime payload that drives the rest of the level scene.
 */
UCLASS()
class DEMOPROJECT_API AHorseView : public AActor {
  GENERATED_BODY()

public:
  AHorseView();

  virtual void Tick(float DeltaTime) override;

  /**
   * @brief Applies store-derived horse presentation data to the actor.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|Horse")
  void ConfigureHorse(const FHorseViewConfig &Config);

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
  float PatrolArrivalDistance;
  bool bMountedRider;

  void AdvancePatrol(float DeltaTime);
  void ConfigureImportedHorseAsset();
  void RefreshText();
};
