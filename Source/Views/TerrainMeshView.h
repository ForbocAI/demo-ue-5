#pragma once

#include "CoreMinimal.h"
#include "Features/Systems/Terrain/TerrainTypes.h"
#include "GameFramework/Actor.h"
#include "TerrainMeshView.generated.h"

class UProceduralMeshComponent;

UCLASS()
class DEMOPROJECT_API ATerrainMeshView : public AActor {
  GENERATED_BODY()

public:
  ATerrainMeshView();

  bool ApplyTerrainMeshPayload(
      const ForbocAI::Demo::Level::FTerrainMeshPayload &Payload);

private:
  UPROPERTY()
  UProceduralMeshComponent *TerrainMesh;
};
