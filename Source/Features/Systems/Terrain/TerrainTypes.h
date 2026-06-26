#pragma once

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

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

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
