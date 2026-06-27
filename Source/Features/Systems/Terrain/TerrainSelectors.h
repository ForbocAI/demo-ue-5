#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainSelectors {

struct FTerrainMeshSectionViewModel {
  bool bLoaded = false;
  TArray<FVector> Vertices;
  TArray<int32> Triangles;
  TArray<FVector> Normals;
  TArray<FVector2D> UVs;
  TArray<FColor> VertexColors;
};

bool SelectLoaded(const FTerrainState &State);
FTerrainMeshSectionViewModel
SelectTerrainMeshSectionViewModel(const FTerrainMeshPayload &Payload);

} // namespace TerrainSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
