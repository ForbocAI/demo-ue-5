#pragma once

#include "CoreMinimal.h"
#include "Features/Components/MapComponents.h"
#include "Features/Systems/Systems.h"
#include "GameFramework/Actor.h"
#include "RuntimeLevel.generated.h"

class AStaticMeshActor;
class ATerrainMesh;
class ATalkableTownsperson;
class AWalkingHorse;
class UMaterialInterface;
class UStaticMesh;

namespace ForbocAI {
namespace Demo {
namespace Map {
enum class EMapRetroTexture : uint8;
struct FMapBlockSpawn;
struct FMapTerrainBlockSpawn;
}
} // namespace Demo
} // namespace ForbocAI

UCLASS()
class DEMOPROJECT_API ARuntimeLevel : public AActor {
  GENERATED_BODY()

public:
  ARuntimeLevel();

  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map|Level")
  bool bSpawnOnBeginPlay;

private:
  UStaticMesh *CubeMesh;
  UMaterialInterface *BlockBaseMaterial;
  FMapTerrainData TerrainData;
  FMapOrthoData OrthoData;
  rtk::EnhancedStore<ForbocAI::Demo::Map::FMapState>
      MapStore;

  void SpawnLevel();
  void SpawnTerrain();
  void SpawnNaturalEnvironment();
  void SpawnTown();
  void SpawnMineApproach();
  void SpawnTownspeople();
  void SpawnHorses();
  void SeedMapStore();

  AStaticMeshActor *SpawnBlock(
      const ForbocAI::Demo::Map::FMapBlockSpawn &BlockSpawn);
  AStaticMeshActor *SpawnTerrainBlock(
      const ForbocAI::Demo::Map::FMapTerrainBlockSpawn
          &BlockSpawn);
  void SpawnTerrainLabel(
      const FString &Text,
      const ForbocAI::Demo::Map::FMapLocalPoint &Point,
      float WorldSize = 42.0f);
  TArray<FVector> BuildRoute(
      const TArray<ForbocAI::Demo::Map::FMapLocalPoint>
          &Route) const;
  void SpawnLabel(const FString &Text, const FVector &Location,
                  float WorldSize = 42.0f);
  ATalkableTownsperson *SpawnTownsperson(
      const FString &Name, const FString &InRole, const FString &Persona,
      const TArray<FVector> &Route);
  AWalkingHorse *SpawnHorse(const FString &Name, const TArray<FVector> &Route,
                            bool bMountedRider);
};
