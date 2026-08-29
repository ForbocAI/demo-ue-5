#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FBinding {
  int32 RuntimePixelMeshSectionIndex;
  int32 RuntimePixelMaterialIndex;
};

inline bool operator==(const FBinding &Left, const FBinding &Right) {
  return Left.RuntimePixelMeshSectionIndex ==
             Right.RuntimePixelMeshSectionIndex &&
         Left.RuntimePixelMaterialIndex == Right.RuntimePixelMaterialIndex;
}

struct FQuadIndices {
  int32 PixelQuadIndexA;
  int32 PixelQuadIndexB;
  int32 PixelQuadIndexC;
  int32 PixelQuadIndexD;
};

inline bool operator==(const FQuadIndices &Left,
                       const FQuadIndices &Right) {
  return Left.PixelQuadIndexA == Right.PixelQuadIndexA &&
         Left.PixelQuadIndexB == Right.PixelQuadIndexB &&
         Left.PixelQuadIndexC == Right.PixelQuadIndexC &&
         Left.PixelQuadIndexD == Right.PixelQuadIndexD;
}

struct FQuadBounds {
  float PixelQuadHalfExtentMultiplier;
  float PixelQuadUvMin;
  float PixelQuadUvMax;
};

inline bool operator==(const FQuadBounds &Left,
                       const FQuadBounds &Right) {
  return FMath::IsNearlyEqual(Left.PixelQuadHalfExtentMultiplier,
                              Right.PixelQuadHalfExtentMultiplier) &&
         FMath::IsNearlyEqual(Left.PixelQuadUvMin, Right.PixelQuadUvMin) &&
         FMath::IsNearlyEqual(Left.PixelQuadUvMax, Right.PixelQuadUvMax);
}

struct FQuad {
  FBinding Binding;
  FQuadIndices Indices;
  FQuadBounds Bounds;
};

inline bool operator==(const FQuad &Left, const FQuad &Right) {
  return Left.Binding == Right.Binding && Left.Indices == Right.Indices &&
         Left.Bounds == Right.Bounds;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
