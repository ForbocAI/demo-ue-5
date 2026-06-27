#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainMeshView.generated.h"

class UProceduralMeshElement;

namespace ForbocAI {
namespace Demo {
namespace Level {
struct FTerrainMeshPayload;
} // namespace Level
} // namespace Demo
} // namespace ForbocAI

UCLASS()
class DEMOPROJECT_API ATerrainMeshView : public AActor {
  GENERATED_BODY()

public:
  ATerrainMeshView();

  bool ApplyTerrainMeshPayload(
      const ForbocAI::Demo::Level::FTerrainMeshPayload &Payload);

private:
  UPROPERTY()
  UProceduralMeshElement *ProceduralMeshElement;
};
