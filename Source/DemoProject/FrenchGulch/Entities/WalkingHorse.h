#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WalkingHorse.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class DEMOPROJECT_API AWalkingHorse : public AActor {
  GENERATED_BODY()

public:
  AWalkingHorse();

  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable, Category = "FrenchGulch|Horse")
  void ConfigureHorse(const FString &InName, const TArray<FVector> &InPatrolRoute);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|Horse")
  FString HorseName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|Horse")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|Horse")
  float WalkSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|Horse")
  float PauseDuration;

private:
  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  USceneComponent *SceneRoot;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *BodyMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *NeckMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *HeadMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *FrontLeftLegMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *FrontRightLegMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *RearLeftLegMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *RearRightLegMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *TailMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UStaticMeshComponent *SaddleMesh;

  UPROPERTY(VisibleAnywhere, Category = "FrenchGulch|Horse")
  UTextRenderComponent *NameText;

  int32 PatrolIndex;
  float PauseRemaining;

  void AdvancePatrol(float DeltaTime);
  void ConfigurePrimitive(UStaticMesh *Mesh);
  void RefreshText();
};
