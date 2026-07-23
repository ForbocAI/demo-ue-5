#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FLoadedPayload {
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
};

struct FLoadedSource {
  FString TerrainSource;
  FString OrthoSource;
  int32 GridSize;
  float MinElevationMeters;
  float MaxElevationMeters;
};

struct FMeshPayload {
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

/** User Story: As a features systems terrain consumer, I need to compare values for equality through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline bool operator==(const FLoadedPayload &Left, const FLoadedPayload &Right) */
inline bool operator==(const FLoadedPayload &Left,
                       const FLoadedPayload &Right) {
  return Left.TerrainSource == Right.TerrainSource &&
         Left.OrthoSource == Right.OrthoSource &&
         Left.GridSize == Right.GridSize &&
         FMath::IsNearlyEqual(Left.MinElevationMeters,
                              Right.MinElevationMeters) &&
         FMath::IsNearlyEqual(Left.MaxElevationMeters,
                              Right.MaxElevationMeters);
}

/** User Story: As a features systems terrain consumer, I need to compare values for inequality through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline bool operator!=(const FLoadedPayload &Left, const FLoadedPayload &Right) */
inline bool operator!=(const FLoadedPayload &Left,
                       const FLoadedPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems terrain consumer, I need to compare values for equality through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline bool operator==(const FLoadedSource &Left, const FLoadedSource &Right) */
inline bool operator==(const FLoadedSource &Left,
                       const FLoadedSource &Right) {
  return Left.TerrainSource == Right.TerrainSource &&
         Left.OrthoSource == Right.OrthoSource &&
         Left.GridSize == Right.GridSize &&
         FMath::IsNearlyEqual(Left.MinElevationMeters,
                              Right.MinElevationMeters) &&
         FMath::IsNearlyEqual(Left.MaxElevationMeters,
                              Right.MaxElevationMeters);
}

/** User Story: As a features systems terrain consumer, I need to compare values for inequality through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline bool operator!=(const FLoadedSource &Left, const FLoadedSource &Right) */
inline bool operator!=(const FLoadedSource &Left,
                       const FLoadedSource &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems terrain consumer, I need to compare values for equality through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline bool operator==(const FTerrainState &Left, const FTerrainState &Right) */
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

/** User Story: As a features systems terrain consumer, I need to compare values for inequality through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline bool operator!=(const FTerrainState &Left, const FTerrainState &Right) */
inline bool operator!=(const FTerrainState &Left,
                       const FTerrainState &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems terrain consumer, I need to compare values for equality through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline bool operator==(const FMeshPayload &Left, const FMeshPayload &Right) */
inline bool operator==(const FMeshPayload &Left,
                       const FMeshPayload &Right) {
  return Left.bLoaded == Right.bLoaded &&
         Left.MaterialSlotIndex == Right.MaterialSlotIndex &&
         Left.MeshSectionIndex == Right.MeshSectionIndex &&
         Left.bCreateCollision == Right.bCreateCollision &&
         Left.Vertices == Right.Vertices &&
         Left.Triangles == Right.Triangles && Left.Normals == Right.Normals &&
         Left.UVs == Right.UVs && Left.VertexColors == Right.VertexColors;
}

/** User Story: As a features systems terrain consumer, I need to compare values for inequality through a stable signature so the features systems terrain workflow remains explicit and composable. @fn inline bool operator!=(const FMeshPayload &Left, const FMeshPayload &Right) */
inline bool operator!=(const FMeshPayload &Left,
                       const FMeshPayload &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
