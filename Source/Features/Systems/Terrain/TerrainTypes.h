#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FTerrainLoadedPayload {
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
};

struct FTerrainLoadedSource {
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
};

struct FTerrainMeshPayload {
  bool bLoaded = false;
  TArray<FVector> Vertices;
  TArray<int32> Triangles;
  TArray<FVector> Normals;
  TArray<FVector2D> UVs;
  TArray<FColor> VertexColors;
};

struct FTerrainMeshSectionViewModel {
  bool bLoaded = false;
  TArray<FVector> Vertices;
  TArray<int32> Triangles;
  TArray<FVector> Normals;
  TArray<FVector2D> UVs;
  TArray<FColor> VertexColors;
};

struct FTerrainState {
  bool bTerrainLoaded = false;
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize = 0;
  float MinElevationMeters = 0.0f;
  float MaxElevationMeters = 0.0f;
  FTerrainMeshSectionViewModel LastMeshSection;
};

inline bool operator==(const FTerrainLoadedPayload &Left,
                       const FTerrainLoadedPayload &Right) {
  return Left.TerrainSource == Right.TerrainSource &&
         Left.OrthoSource == Right.OrthoSource &&
         Left.GridSize == Right.GridSize &&
         FMath::IsNearlyEqual(Left.MinElevationMeters,
                              Right.MinElevationMeters) &&
         FMath::IsNearlyEqual(Left.MaxElevationMeters,
                              Right.MaxElevationMeters);
}

inline bool operator!=(const FTerrainLoadedPayload &Left,
                       const FTerrainLoadedPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTerrainLoadedSource &Left,
                       const FTerrainLoadedSource &Right) {
  return Left.TerrainSource == Right.TerrainSource &&
         Left.OrthoSource == Right.OrthoSource &&
         Left.GridSize == Right.GridSize &&
         FMath::IsNearlyEqual(Left.MinElevationMeters,
                              Right.MinElevationMeters) &&
         FMath::IsNearlyEqual(Left.MaxElevationMeters,
                              Right.MaxElevationMeters);
}

inline bool operator!=(const FTerrainLoadedSource &Left,
                       const FTerrainLoadedSource &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTerrainState &Left,
                       const FTerrainState &Right) {
  return Left.bTerrainLoaded == Right.bTerrainLoaded &&
         Left.TerrainSource == Right.TerrainSource &&
         Left.OrthoSource == Right.OrthoSource &&
         Left.GridSize == Right.GridSize &&
         FMath::IsNearlyEqual(Left.MinElevationMeters,
                              Right.MinElevationMeters) &&
         FMath::IsNearlyEqual(Left.MaxElevationMeters,
                              Right.MaxElevationMeters) &&
         Left.LastMeshSection.bLoaded == Right.LastMeshSection.bLoaded &&
         Left.LastMeshSection.Vertices == Right.LastMeshSection.Vertices &&
         Left.LastMeshSection.Triangles == Right.LastMeshSection.Triangles &&
         Left.LastMeshSection.Normals == Right.LastMeshSection.Normals &&
         Left.LastMeshSection.UVs == Right.LastMeshSection.UVs &&
         Left.LastMeshSection.VertexColors ==
             Right.LastMeshSection.VertexColors;
}

inline bool operator!=(const FTerrainState &Left,
                       const FTerrainState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTerrainMeshPayload &Left,
                       const FTerrainMeshPayload &Right) {
  return Left.bLoaded == Right.bLoaded && Left.Vertices == Right.Vertices &&
         Left.Triangles == Right.Triangles && Left.Normals == Right.Normals &&
         Left.UVs == Right.UVs && Left.VertexColors == Right.VertexColors;
}

inline bool operator!=(const FTerrainMeshPayload &Left,
                       const FTerrainMeshPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTerrainMeshSectionViewModel &Left,
                       const FTerrainMeshSectionViewModel &Right) {
  return Left.bLoaded == Right.bLoaded && Left.Vertices == Right.Vertices &&
         Left.Triangles == Right.Triangles && Left.Normals == Right.Normals &&
         Left.UVs == Right.UVs && Left.VertexColors == Right.VertexColors;
}

inline bool operator!=(const FTerrainMeshSectionViewModel &Left,
                       const FTerrainMeshSectionViewModel &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
