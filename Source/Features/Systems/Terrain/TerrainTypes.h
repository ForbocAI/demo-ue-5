#pragma once

#include "Core/rtk.hpp"

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Demo {
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

struct FTerrainState {
  bool bTerrainLoaded;
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
  return Left.bLoaded == Right.bLoaded && Left.Vertices == Right.Vertices &&
         Left.Triangles == Right.Triangles && Left.Normals == Right.Normals &&
         Left.UVs == Right.UVs && Left.VertexColors == Right.VertexColors;
}

inline bool operator!=(const FTerrainMeshPayload &Left,
                       const FTerrainMeshPayload &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
