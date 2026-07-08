#pragma once

#include "Features/Systems/Rendering/Profile/Sky/Color/ColorSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileSkyReducers {
FRuntimePixelQuad ReduceMoonPixelQuad(const FLevelRetroRenderProfile &Profile,
                                      const func::GridIndex &Index) {
  return {ReduceMoonDiscLocation(Profile) +
              MoonPixelOffset(Profile, Index),
          MoonPixelPlaneRight(Profile), MoonPixelPlaneUp(Profile),
          MoonPixelCellWorldSize(Profile) *
              Profile.MoonPixelOverlapMultiplier,
          MoonDiscColor(Profile)};
}

FRuntimePixelMeshBuffers ReduceMoonPixelMeshBuffers(
    const FLevelRetroRenderProfile &Profile) {
  FRuntimePixelMeshBuffers Buffers;
  func::for_each_array<func::GridIndex>(
      ReduceMoonPixelIndices(Profile),
      [&Profile, &Buffers](const func::GridIndex &Index) {
        AppendPixelQuad(Buffers, Profile, ReduceMoonPixelQuad(Profile, Index));
      });
  return Buffers;
}

// --- Point stars reducers ------------------------------------------------

FRuntimePixelQuad ReducePointStarQuad(const FLevelRetroRenderProfile &Profile,
                                      int32 Index) {
  return {PointStarLocation(Profile, Index),
          PointStarPlaneRight(Profile, Index),
          PointStarPlaneUp(Profile, Index),
          PointStarWorldSize(Profile, Index), SkyDomeStarColor(Profile)};
}

FRuntimePixelMeshBuffers ReducePointStarMeshBuffers(
    const FLevelRetroRenderProfile &Profile) {
  FRuntimePixelMeshBuffers Buffers;
  func::for_each_array<int32>(
      func::index_range(Profile.PointStarCount),
      [&Profile, &Buffers](const int32 &Index) {
        AppendPixelQuad(Buffers, Profile, ReducePointStarQuad(Profile, Index));
      });
  return Buffers;
}

// --- Shared pixel mesh reducer -------------------------------------------

FVector ReducePixelQuadNormal(const FRuntimePixelQuad &Quad) {
  return FVector::CrossProduct(Quad.Right, Quad.Up).GetSafeNormal();
}

void AppendPixelQuad(FRuntimePixelMeshBuffers &Buffers,
                     const FLevelRetroRenderProfile &Profile,
                     const FRuntimePixelQuad &Quad) {
  const int32 BaseIndex = Buffers.Vertices.Num();
  const FVector Right =
      Quad.Right * (Quad.Size * Profile.PixelQuadHalfExtentMultiplier);
  const FVector Up =
      Quad.Up * (Quad.Size * Profile.PixelQuadHalfExtentMultiplier);
  const FVector Normal = ReducePixelQuadNormal(Quad);
  const int32 A = BaseIndex + Profile.PixelQuadIndexA;
  const int32 B = BaseIndex + Profile.PixelQuadIndexB;
  const int32 C = BaseIndex + Profile.PixelQuadIndexC;
  const int32 D = BaseIndex + Profile.PixelQuadIndexD;
  Buffers.Vertices.Append(
      {Quad.Center - Right - Up, Quad.Center + Right - Up,
       Quad.Center + Right + Up, Quad.Center - Right + Up});
  Buffers.Triangles.Append({A, B, C, A, C, D, C, B, A, D, C, A});
  Buffers.Normals.Append({Normal, Normal, Normal, Normal});
  Buffers.UV0.Append(
      {FVector2D(Profile.PixelQuadUvMin, Profile.PixelQuadUvMin),
       FVector2D(Profile.PixelQuadUvMax, Profile.PixelQuadUvMin),
       FVector2D(Profile.PixelQuadUvMax, Profile.PixelQuadUvMax),
       FVector2D(Profile.PixelQuadUvMin, Profile.PixelQuadUvMax)});
  Buffers.VertexColors.Append({Quad.Color, Quad.Color, Quad.Color,
                               Quad.Color});
  Buffers.Tangents.Append({FProcMeshTangent(Quad.Right, false),
                           FProcMeshTangent(Quad.Right, false),
                           FProcMeshTangent(Quad.Right, false),
                           FProcMeshTangent(Quad.Right, false)});
}
} // namespace RenderingProfileSkyReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
