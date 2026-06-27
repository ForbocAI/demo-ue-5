#include "Views/TerrainMeshView.h"

#include "ProceduralMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ATerrainMeshView::ATerrainMeshView() {
  PrimaryActorTick.bCanEverTick = false;

  TerrainMesh =
      CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
  RootComponent = TerrainMesh;
  TerrainMesh->bUseAsyncCooking = true;
  TerrainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

  static ConstructorHelpers::FObjectFinder<UMaterialInterface> VertexMaterial(
      TEXT("/Engine/EngineDebugMaterials/VertexColorMaterial.VertexColorMaterial"));
  VertexMaterial.Succeeded() ? (TerrainMesh->SetMaterial(0, VertexMaterial.Object),
                                void())
                             : void();
}

bool ATerrainMeshView::ApplyTerrainMeshPayload(
    const ForbocAI::Demo::Level::FTerrainMeshPayload &Payload) {
  TArray<FProcMeshTangent> Tangents;
  return Payload.bLoaded
             ? (TerrainMesh->CreateMeshSection(
                    0, Payload.Vertices, Payload.Triangles, Payload.Normals,
                    Payload.UVs, Payload.VertexColors, Tangents, true),
                true)
             : false;
}
