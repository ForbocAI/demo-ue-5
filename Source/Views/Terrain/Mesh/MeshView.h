// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshView.generated.h"
class UProceduralMeshElement;
class UMaterialInterface;

namespace ForbocAI {
namespace Game {
namespace Level {
struct FMeshPayload;
} // namespace Level
} // namespace Game
} // namespace ForbocAI

/**
 * @brief Procedural terrain display actor for reducer/thunk-owned mesh data.
 *
 * Architecture: Terrain systems select or load FMeshPayload below the
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
  /** User Story: As a views terrain mesh consumer, I need to invoke aterrain mesh view through a stable signature so the views terrain mesh workflow remains explicit and composable. @fn ATerrainMeshView() */
  ATerrainMeshView();

  /**
   * @fn bool ApplyTerrainMeshPayload( const ForbocAI::Game::Level::FMeshPayload &Payload)
   * @brief Applies one terrain mesh payload to the procedural mesh component.
   *
   * @return true when the mesh section was created.
   * User Story: As a views terrain mesh consumer, I need to invoke apply terrain mesh payload through a stable signature so the views terrain mesh workflow remains explicit and composable.
   */
  bool ApplyTerrainMeshPayload(
      const ForbocAI::Game::Level::FMeshPayload &Payload);

private:
  UPROPERTY()
  UProceduralMeshElement *ProceduralMeshElement;

  UPROPERTY()
  UMaterialInterface *TerrainMaterial;
};
