#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrenchGulch/Components/FrenchGulchOrthoData.h"
#include "FrenchGulch/Components/FrenchGulchTerrainData.h"
#include "FrenchGulchTerrainMesh.generated.h"

class UProceduralMeshComponent;

UCLASS()
class DEMOPROJECT_API AFrenchGulchTerrainMesh : public AActor {
  GENERATED_BODY()

public:
  AFrenchGulchTerrainMesh();

  bool BuildFromTerrainData(const FFrenchGulchTerrainData &TerrainData,
                            const FFrenchGulchOrthoData &OrthoData);

private:
  UPROPERTY()
  UProceduralMeshComponent *TerrainMesh;
};
