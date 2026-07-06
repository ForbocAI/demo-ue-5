// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainMeshView.generated.h"

class UProceduralMeshElement;
class UMaterialInterface;

namespace ForbocAI {
namespace Game {
namespace Level {
struct FTerrainMeshPayload;
} // namespace Level
} // namespace Game
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
class FORBOCAIDEMO_API ATerrainMeshView : public AActor {
  GENERATED_BODY()

public:
  ATerrainMeshView();

  /**
   * @brief Applies one terrain mesh payload to the procedural mesh component.
   *
   * @return true when the mesh section was created.
   */
  bool ApplyTerrainMeshPayload(
      const ForbocAI::Game::Level::FTerrainMeshPayload &Payload);

private:
  UPROPERTY()
  UProceduralMeshElement *ProceduralMeshElement;

  UPROPERTY()
  UMaterialInterface *TerrainMaterial;
};
