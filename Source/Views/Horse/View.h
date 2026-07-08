// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "Features/Systems/Selectors.h"
#include "GameFramework/Actor.h"
#include "View.generated.h"

class USkeletalMeshComponent;
class UTextRenderComponent;

/**
 * @brief View payload for configuring one horse actor from runtime store data.
 *
 * Architecture: Runtime reducers prepare horse spawn presentation data; this
 * struct is an Unreal-facing adapter shape for Feature-owned store semantics.
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

  ForbocAI::Game::Level::FLevelDistanceLodStage Lod;
};

/**
 * @brief Display actor for a reducer-owned horse route payload.
 *
 * Architecture: The actor applies configured presentation and patrol movement.
 * Route and mounted-rider facts originate in feature reducers/thunks, keeping
 * ECS/store data below the view layer.
 *
 * User story: As a player exploring the runtime town, horses appear from the same
 * runtime payload that drives the rest of the level scene.
 */
UCLASS()
class FORBOCAIDEMO_API AHorseView : public AActor {
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
  FString HorseMeshPath;
  FString HorseWalkAnimationPath;
  FString RiderMeshPath;
  FString RiderWalkAnimationPath;
  ForbocAI::Game::Level::FLevelDistanceLodStage CurrentLod;

  void AdvancePatrol(float DeltaTime);
  void ApplyDistanceLod(
      const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod);
  void ConfigureImportedHorseAsset();
  void RefreshText();
};
