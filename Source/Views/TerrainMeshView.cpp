// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/TerrainMeshView.h"

#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Terrain/TerrainActions.h"
#include "Store.h"
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
  VertexMaterial.Succeeded() ? (ProceduralMeshElement->SetMaterial(
                                    0, VertexMaterial.Object),
                                void())
                             : void();
}

bool ATerrainMeshView::ApplyTerrainMeshPayload(
    const ForbocAI::Game::Level::FTerrainMeshPayload &Payload) {
  FG::Store::GetStore().dispatch(
      FG::TerrainActions::TerrainMeshPayloadObserved()(Payload));
  const FG::FTerrainMeshSectionViewModel Model =
      FG::RuntimeSelectors::SelectTerrainMeshSectionViewModel(
          FG::Store::GetStore().getState());
  TArray<FProcMeshTangent> Tangents;
  return Model.bLoaded
             ? (ProceduralMeshElement->CreateMeshSection(
                    0, Model.Vertices, Model.Triangles, Model.Normals,
                    Model.UVs, Model.VertexColors, Tangents, true),
                true)
             : false;
}
