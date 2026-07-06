// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshElement.generated.h"

/**
 * @brief Thin Unreal component wrapper used by TerrainMeshView.
 *
 * Architecture: Kept as a display primitive below view actors. Terrain
 * reducers/thunks provide mesh topology for this Unreal procedural mesh.
 */
UCLASS()
class FORBOCAIDEMO_API UProceduralMeshElement : public UProceduralMeshComponent {
  GENERATED_BODY()
};
