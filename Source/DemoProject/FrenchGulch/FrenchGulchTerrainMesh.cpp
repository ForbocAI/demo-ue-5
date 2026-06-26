#include "FrenchGulch/FrenchGulchTerrainMesh.h"

#include "ProceduralMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

AFrenchGulchTerrainMesh::AFrenchGulchTerrainMesh() {
  PrimaryActorTick.bCanEverTick = false;

  TerrainMesh =
      CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("TerrainMesh"));
  RootComponent = TerrainMesh;
  TerrainMesh->bUseAsyncCooking = true;
  TerrainMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

  static ConstructorHelpers::FObjectFinder<UMaterialInterface> VertexMaterial(
      TEXT("/Engine/EngineDebugMaterials/VertexColorMaterial.VertexColorMaterial"));
  if (VertexMaterial.Succeeded()) {
    TerrainMesh->SetMaterial(0, VertexMaterial.Object);
  }
}

bool AFrenchGulchTerrainMesh::BuildFromTerrainData(
    const FFrenchGulchTerrainData &TerrainData,
    const FFrenchGulchOrthoData &OrthoData) {
  if (!TerrainData.IsLoaded()) {
    return false;
  }

  const int32 GridSize = TerrainData.GetGridSize();
  const float HalfSize = FFrenchGulchTerrainData::TerrainWorldSize * 0.5f;
  const float Step =
      FFrenchGulchTerrainData::TerrainWorldSize / static_cast<float>(GridSize - 1);

  TArray<FVector> Vertices;
  TArray<int32> Triangles;
  TArray<FVector> Normals;
  TArray<FVector2D> UVs;
  TArray<FColor> VertexColors;
  TArray<FProcMeshTangent> Tangents;

  Vertices.Reserve(GridSize * GridSize);
  Normals.Reserve(GridSize * GridSize);
  UVs.Reserve(GridSize * GridSize);
  VertexColors.Reserve(GridSize * GridSize);
  Tangents.Reserve(GridSize * GridSize);
  Triangles.Reserve((GridSize - 1) * (GridSize - 1) * 6);

  for (int32 Row = 0; Row < GridSize; ++Row) {
    const float NorthSouth = HalfSize - Step * Row;
    for (int32 Column = 0; Column < GridSize; ++Column) {
      const float EastWest = -HalfSize + Step * Column;
      Vertices.Add(TerrainData.ToWorld(EastWest, NorthSouth, 0.0f));
      Normals.Add(FVector::UpVector);
      UVs.Add(FVector2D(static_cast<float>(Column) / (GridSize - 1),
                        static_cast<float>(Row) / (GridSize - 1)));
      VertexColors.Add(OrthoData.IsLoaded() ? OrthoData.GetColorAt(Row, Column)
                                            : FColor(94, 111, 78));
      Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
    }
  }

  for (int32 Row = 0; Row < GridSize - 1; ++Row) {
    for (int32 Column = 0; Column < GridSize - 1; ++Column) {
      const int32 A = Row * GridSize + Column;
      const int32 B = A + 1;
      const int32 C = A + GridSize;
      const int32 D = C + 1;

      Triangles.Add(A);
      Triangles.Add(C);
      Triangles.Add(B);
      Triangles.Add(B);
      Triangles.Add(C);
      Triangles.Add(D);
    }
  }

  TerrainMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs,
                                 VertexColors, Tangents, true);
  return true;
}
