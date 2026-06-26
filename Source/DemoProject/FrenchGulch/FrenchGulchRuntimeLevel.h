#pragma once

#include "CoreMinimal.h"
#include "FrenchGulch/Components/FrenchGulchComponents.h"
#include "FrenchGulch/Systems/FrenchGulchSystems.h"
#include "GameFramework/Actor.h"
#include "FrenchGulchRuntimeLevel.generated.h"

class AStaticMeshActor;
class AFrenchGulchTerrainMesh;
class ATalkableTownsperson;
class AWalkingHorse;
class UStaticMesh;

UCLASS()
class DEMOPROJECT_API AFrenchGulchRuntimeLevel : public AActor {
  GENERATED_BODY()

public:
  AFrenchGulchRuntimeLevel();

  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FrenchGulch|Level")
  bool bSpawnOnBeginPlay;

private:
  UStaticMesh *CubeMesh;
  FFrenchGulchTerrainData TerrainData;
  FFrenchGulchOrthoData OrthoData;
  rtk::EnhancedStore<ForbocAI::Demo::FrenchGulch::FFrenchGulchMapState>
      MapStore;

  void SpawnLevel();
  void SpawnTerrain();
  void SpawnNaturalEnvironment();
  void SpawnTown();
  void SpawnMineApproach();
  void SpawnTownspeople();
  void SpawnHorses();
  void SeedMapStore();

  AStaticMeshActor *SpawnBlock(const FString &Name, const FVector &Location,
                               const FVector &Scale);
  AStaticMeshActor *SpawnTerrainBlock(
      const FString &Name,
      const ForbocAI::Demo::FrenchGulch::FFrenchGulchLocalPoint &Point,
      const FVector &Scale);
  void SpawnTerrainLabel(
      const FString &Text,
      const ForbocAI::Demo::FrenchGulch::FFrenchGulchLocalPoint &Point,
      float WorldSize = 42.0f);
  TArray<FVector> BuildRoute(
      const TArray<ForbocAI::Demo::FrenchGulch::FFrenchGulchLocalPoint>
          &Route) const;
  void SpawnLabel(const FString &Text, const FVector &Location,
                  float WorldSize = 42.0f);
  ATalkableTownsperson *SpawnTownsperson(
      const FString &Name, const FString &InRole, const FString &Persona,
      const TArray<FVector> &Route);
  AWalkingHorse *SpawnHorse(const FString &Name, const TArray<FVector> &Route);
};
