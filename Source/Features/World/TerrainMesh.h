#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Features/Components/OrthoData.h"
#include "Features/Components/TerrainData.h"
#include "TerrainMesh.generated.h"

class UProceduralMeshComponent;

UCLASS()
class DEMOPROJECT_API ATerrainMesh : public AActor {
  GENERATED_BODY()

public:
  ATerrainMesh();

  bool BuildFromTerrainData(const FMapTerrainData &TerrainData,
                            const FMapOrthoData &OrthoData);

private:
  UPROPERTY()
  UProceduralMeshComponent *TerrainMesh;
};
