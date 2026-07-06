// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/TerrainMeshView.h"

#include "Features/Systems/Terrain/TerrainTypes.h"
#include "Views/ProceduralMeshElement.h"
#include "UObject/ConstructorHelpers.h"

namespace FG = ForbocAI::Game::Level;

ATerrainMeshView::ATerrainMeshView() {
  PrimaryActorTick.bCanEverTick = false;

  ProceduralMeshElement =
      CreateDefaultSubobject<UProceduralMeshElement>(
          TEXT("ProceduralMeshElement"));
  RootComponent = ProceduralMeshElement;
  ProceduralMeshElement->bUseAsyncCooking = true;
  ProceduralMeshElement->SetCollisionEnabled(
      ECollisionEnabled::QueryAndPhysics);

  static ConstructorHelpers::FObjectFinder<UMaterialInterface> VertexMaterial(
      TEXT("/Engine/EngineDebugMaterials/VertexColorMaterial.VertexColorMaterial"));
  TerrainMaterial =
      VertexMaterial.Succeeded() ? VertexMaterial.Object : nullptr;
}

bool ATerrainMeshView::ApplyTerrainMeshPayload(
    const ForbocAI::Game::Level::FTerrainMeshPayload &Payload) {
  TArray<FProcMeshTangent> Tangents;
  return Payload.bLoaded
             ? (TerrainMaterial
                    ? (ProceduralMeshElement->SetMaterial(
                           Payload.MaterialSlotIndex, TerrainMaterial),
                       void())
                    : void(),
                ProceduralMeshElement->CreateMeshSection(
                    Payload.MeshSectionIndex, Payload.Vertices,
                    Payload.Triangles, Payload.Normals, Payload.UVs,
                    Payload.VertexColors, Tangents, Payload.bCreateCollision),
                true)
             : false;
}
