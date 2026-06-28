#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshElement.generated.h"

/**
 * @brief Thin Unreal component wrapper used by TerrainMeshView.
 *
 * Architecture: Kept as a display primitive below view actors. Mesh topology
 * comes from terrain reducers/thunks; this component owns no domain logic.
 */
UCLASS()
class DEMOPROJECT_API UProceduralMeshElement : public UProceduralMeshComponent {
  GENERATED_BODY()
};
