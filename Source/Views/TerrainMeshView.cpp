#include "Views/TerrainMeshView.h"

#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Views/ProceduralMeshElement.h"
#include "UObject/ConstructorHelpers.h"

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
  VertexMaterial.Succeeded() ? (ProceduralMeshElement->SetMaterial(
                                    0, VertexMaterial.Object),
                                void())
                             : void();
}

bool ATerrainMeshView::ApplyTerrainMeshPayload(
    const ForbocAI::Demo::Level::FTerrainMeshPayload &Payload) {
  const ForbocAI::Demo::Level::TerrainSelectors::FTerrainMeshSectionViewModel
      Model = ForbocAI::Demo::Level::TerrainSelectors::
          SelectTerrainMeshSectionViewModel(Payload);
  TArray<FProcMeshTangent> Tangents;
  return Model.bLoaded
             ? (ProceduralMeshElement->CreateMeshSection(
                    0, Model.Vertices, Model.Triangles, Model.Normals,
                    Model.UVs, Model.VertexColors, Tangents, true),
                true)
             : false;
}
