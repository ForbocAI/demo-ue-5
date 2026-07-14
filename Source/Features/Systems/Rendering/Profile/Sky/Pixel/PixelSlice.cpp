#include "Features/Systems/Rendering/Profile/Sky/SkySlice.h"

#include "ProceduralMeshComponent.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileSkyReducers {

namespace {

float MoonPixelGridCenter(const FLevelRetroRenderProfile &Profile) {
  return static_cast<float>(Profile.MoonPixelGridSize -
                            Profile.MoonPixelGridTerminalOffset) *
         Profile.PixelQuadHalfExtentMultiplier;
}

float MoonPixelCoordinate(const FLevelRetroRenderProfile &Profile,
                          int32 Pixel) {
  return (static_cast<float>(Pixel) - MoonPixelGridCenter(Profile)) /
         MoonPixelGridCenter(Profile);
}

bool MoonPixelVisible(const FLevelRetroRenderProfile &Profile,
                      const func::GridIndex &Index) {
  const float X =
      MoonPixelCoordinate(Profile, static_cast<int32>(Index.Column));
  const float Y = MoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
  return FVector2D(X, Y).Size() <= Profile.MoonPixelVisibleRadius;
}

float MoonPixelWorldDiameter(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonDiscScale * Profile.MoonDiscWorldUnitsPerScale;
}

float MoonPixelCellWorldSize(const FLevelRetroRenderProfile &Profile) {
  return MoonPixelWorldDiameter(Profile) /
         static_cast<float>(Profile.MoonPixelGridSize);
}

FVector MoonPixelPlaneRight(const FLevelRetroRenderProfile &Profile) {
  return FRotationMatrix(ReduceMoonDiscRotation(Profile))
      .GetScaledAxis(EAxis::X);
}

FVector MoonPixelPlaneUp(const FLevelRetroRenderProfile &Profile) {
  return FRotationMatrix(ReduceMoonDiscRotation(Profile))
      .GetScaledAxis(EAxis::Y);
}

FVector MoonPixelOffset(const FLevelRetroRenderProfile &Profile,
                        const func::GridIndex &Index) {
  const float X =
      MoonPixelCoordinate(Profile, static_cast<int32>(Index.Column));
  const float Y = MoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
  const float Radius =
      MoonPixelWorldDiameter(Profile) * Profile.PixelQuadHalfExtentMultiplier;
  return MoonPixelPlaneRight(Profile) * (X * Radius) +
         MoonPixelPlaneUp(Profile) * (Y * Radius);
}

struct FPointStarHashRequest {
  const FLevelRetroRenderProfile *Profile;
  int32 Index;
  float Salt;
};

float PointStarHash(const FPointStarHashRequest &Request) {
  const FLevelRetroRenderProfile &Profile = *Request.Profile;
  return FMath::Frac(
      FMath::Sin((static_cast<float>(Request.Index) +
                  Profile.PointStarHashIndexOffset) *
                 Request.Salt) *
      Profile.PointStarHashMultiplier);
}

float PointStarYawDegrees(const FLevelRetroRenderProfile &Profile,
                          int32 Index) {
  return PointStarHash({&Profile, Index, Profile.PointStarYawHashSalt}) *
         Profile.PointStarYawSpanDegrees;
}

float PointStarPitchDegrees(const FLevelRetroRenderProfile &Profile,
                            int32 Index) {
  return Profile.PointStarPitchMinDegrees +
         PointStarHash({&Profile, Index, Profile.PointStarPitchHashSalt}) *
             Profile.PointStarPitchSpanDegrees;
}

float PointStarDistance(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonDiscDistance * Profile.PointStarDistanceMultiplier;
}

FVector PointStarLocation(const FLevelRetroRenderProfile &Profile,
                          int32 Index) {
  return FRotator(PointStarPitchDegrees(Profile, Index),
                  PointStarYawDegrees(Profile, Index),
                  Profile.PointStarRollDegrees)
             .Vector() *
         PointStarDistance(Profile);
}

float PointStarWorldSize(const FLevelRetroRenderProfile &Profile,
                         int32 Index) {
  return Profile.PointStarWorldSizeMin +
         PointStarHash({&Profile, Index, Profile.PointStarSizeHashSalt}) *
             Profile.PointStarWorldSizeJitter;
}

FRotator PointStarRotation(const FLevelRetroRenderProfile &Profile,
                           int32 Index) {
  return FRotationMatrix::MakeFromZ(
             -PointStarLocation(Profile, Index).GetSafeNormal())
      .Rotator();
}

FVector PointStarPlaneRight(const FLevelRetroRenderProfile &Profile,
                            int32 Index) {
  return FRotationMatrix(PointStarRotation(Profile, Index))
      .GetScaledAxis(EAxis::X);
}

FVector PointStarPlaneUp(const FLevelRetroRenderProfile &Profile,
                         int32 Index) {
  return FRotationMatrix(PointStarRotation(Profile, Index))
      .GetScaledAxis(EAxis::Y);
}

} // namespace

TArray<func::GridIndex> ReduceMoonPixelIndices(
    const FLevelRetroRenderProfile &Profile) {
  return func::filter_array<func::GridIndex>(
      func::map_grid_array<func::GridIndex>(
          static_cast<size_t>(Profile.MoonPixelGridSize),
          static_cast<size_t>(Profile.MoonPixelGridSize),
          [](const func::GridIndex &Index) { return Index; }),
      [&Profile](const func::GridIndex &Index) {
        return MoonPixelVisible(Profile, Index);
      });
}

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
