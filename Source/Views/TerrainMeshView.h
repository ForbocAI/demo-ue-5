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

/**
 * @brief Procedural terrain display actor for reducer/thunk-owned mesh data.
 *
 * Architecture: Terrain systems select or load FTerrainMeshPayload below the
 * view layer. This actor only applies the payload to Unreal procedural mesh
 * components.
 *
 * User story: As a player, the runtime terrain renders from authoritative
 * store data without duplicating terrain decisions in the actor.
 */
UCLASS()
class DEMOPROJECT_API ATerrainMeshView : public AActor {
  GENERATED_BODY()

public:
  ATerrainMeshView();

  /**
   * @brief Applies one terrain mesh payload to the procedural mesh component.
   *
   * @return true when the mesh section was created.
   */
  bool ApplyTerrainMeshPayload(
      const ForbocAI::Demo::Level::FTerrainMeshPayload &Payload);

private:
  UPROPERTY()
  UProceduralMeshElement *ProceduralMeshElement;
};
