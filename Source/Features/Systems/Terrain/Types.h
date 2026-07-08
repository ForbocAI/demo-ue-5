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
  int32 MaterialSlotIndex = int32{};
  int32 MeshSectionIndex = int32{};
  bool bCreateCollision = false;
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
  int32 GridSize = int32{};
  float MinElevationMeters = float{};
  float MaxElevationMeters = float{};
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
                              Right.MaxElevationMeters);
}

inline bool operator!=(const FTerrainState &Left,
                       const FTerrainState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTerrainMeshPayload &Left,
                       const FTerrainMeshPayload &Right) {
  return Left.bLoaded == Right.bLoaded &&
         Left.MaterialSlotIndex == Right.MaterialSlotIndex &&
         Left.MeshSectionIndex == Right.MeshSectionIndex &&
         Left.bCreateCollision == Right.bCreateCollision &&
         Left.Vertices == Right.Vertices &&
         Left.Triangles == Right.Triangles && Left.Normals == Right.Normals &&
         Left.UVs == Right.UVs && Left.VertexColors == Right.VertexColors;
}

inline bool operator!=(const FTerrainMeshPayload &Left,
                       const FTerrainMeshPayload &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
