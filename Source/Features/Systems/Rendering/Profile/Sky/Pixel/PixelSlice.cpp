#include "Features/Systems/Rendering/Profile/Sky/SkySlice.h"

#include "ProceduralMeshComponent.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileSkyReducers {

namespace {

/** User Story: As a profile sky pixel consumer, I need to invoke moon pixel grid center through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float MoonPixelGridCenter(const FLevelRetroRenderProfile &Profile) */
float MoonPixelGridCenter(const FLevelRetroRenderProfile &Profile) {
  return static_cast<float>(Profile.MoonPixelGridSize -
                            Profile.MoonPixelGridTerminalOffset) *
         Profile.PixelQuadHalfExtentMultiplier;
}

/** User Story: As a profile sky pixel consumer, I need to invoke moon pixel coordinate through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float MoonPixelCoordinate(const FLevelRetroRenderProfile &Profile, int32 Pixel) */
float MoonPixelCoordinate(const FLevelRetroRenderProfile &Profile,
                          int32 Pixel) {
  return (static_cast<float>(Pixel) - MoonPixelGridCenter(Profile)) /
         MoonPixelGridCenter(Profile);
}

/** User Story: As a profile sky pixel consumer, I need to invoke moon pixel visible through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn bool MoonPixelVisible(const FLevelRetroRenderProfile &Profile, const func::GridIndex &Index) */
bool MoonPixelVisible(const FLevelRetroRenderProfile &Profile,
                      const func::GridIndex &Index) {
  const float X =
      MoonPixelCoordinate(Profile, static_cast<int32>(Index.Column));
  const float Y = MoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
  return FVector2D(X, Y).Size() <= Profile.MoonPixelVisibleRadius;
}

/** User Story: As a profile sky pixel consumer, I need to invoke moon pixel world diameter through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float MoonPixelWorldDiameter(const FLevelRetroRenderProfile &Profile) */
float MoonPixelWorldDiameter(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonDiscScale * Profile.MoonDiscWorldUnitsPerScale;
}

/** User Story: As a profile sky pixel consumer, I need to invoke moon pixel cell world size through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float MoonPixelCellWorldSize(const FLevelRetroRenderProfile &Profile) */
float MoonPixelCellWorldSize(const FLevelRetroRenderProfile &Profile) {
  return MoonPixelWorldDiameter(Profile) /
         static_cast<float>(Profile.MoonPixelGridSize);
}

/** User Story: As a profile sky pixel consumer, I need to invoke moon pixel plane right through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FVector MoonPixelPlaneRight(const FLevelRetroRenderProfile &Profile) */
FVector MoonPixelPlaneRight(const FLevelRetroRenderProfile &Profile) {
  return FRotationMatrix(ReduceMoonDiscRotation(Profile))
      .GetScaledAxis(EAxis::X);
}

/** User Story: As a profile sky pixel consumer, I need to invoke moon pixel plane up through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FVector MoonPixelPlaneUp(const FLevelRetroRenderProfile &Profile) */
FVector MoonPixelPlaneUp(const FLevelRetroRenderProfile &Profile) {
  return FRotationMatrix(ReduceMoonDiscRotation(Profile))
      .GetScaledAxis(EAxis::Y);
}

/** User Story: As a profile sky pixel consumer, I need to invoke moon pixel offset through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FVector MoonPixelOffset(const FLevelRetroRenderProfile &Profile, const func::GridIndex &Index) */
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

/** User Story: As a profile sky pixel consumer, I need to invoke point star hash through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float PointStarHash(const FPointStarHashRequest &Request) */
float PointStarHash(const FPointStarHashRequest &Request) {
  const FLevelRetroRenderProfile &Profile = *Request.Profile;
  return FMath::Frac(
      FMath::Sin((static_cast<float>(Request.Index) +
                  Profile.PointStarHashIndexOffset) *
                 Request.Salt) *
      Profile.PointStarHashMultiplier);
}

/** User Story: As a profile sky pixel consumer, I need to invoke point star yaw degrees through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float PointStarYawDegrees(const FLevelRetroRenderProfile &Profile, int32 Index) */
float PointStarYawDegrees(const FLevelRetroRenderProfile &Profile,
                          int32 Index) {
  return PointStarHash({&Profile, Index, Profile.PointStarYawHashSalt}) *
         Profile.PointStarYawSpanDegrees;
}

/** User Story: As a profile sky pixel consumer, I need to invoke point star pitch degrees through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float PointStarPitchDegrees(const FLevelRetroRenderProfile &Profile, int32 Index) */
float PointStarPitchDegrees(const FLevelRetroRenderProfile &Profile,
                            int32 Index) {
  return Profile.PointStarPitchMinDegrees +
         PointStarHash({&Profile, Index, Profile.PointStarPitchHashSalt}) *
             Profile.PointStarPitchSpanDegrees;
}

/** User Story: As a profile sky pixel consumer, I need to invoke point star distance through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float PointStarDistance(const FLevelRetroRenderProfile &Profile) */
float PointStarDistance(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonDiscDistance * Profile.PointStarDistanceMultiplier;
}

/** User Story: As a profile sky pixel consumer, I need to invoke point star location through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FVector PointStarLocation(const FLevelRetroRenderProfile &Profile, int32 Index) */
FVector PointStarLocation(const FLevelRetroRenderProfile &Profile,
                          int32 Index) {
  return FRotator(PointStarPitchDegrees(Profile, Index),
                  PointStarYawDegrees(Profile, Index),
                  Profile.PointStarRollDegrees)
             .Vector() *
         PointStarDistance(Profile);
}

/** User Story: As a profile sky pixel consumer, I need to invoke point star world size through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn float PointStarWorldSize(const FLevelRetroRenderProfile &Profile, int32 Index) */
float PointStarWorldSize(const FLevelRetroRenderProfile &Profile,
                         int32 Index) {
  return Profile.PointStarWorldSizeMin +
         PointStarHash({&Profile, Index, Profile.PointStarSizeHashSalt}) *
             Profile.PointStarWorldSizeJitter;
}

/** User Story: As a profile sky pixel consumer, I need to invoke point star rotation through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FRotator PointStarRotation(const FLevelRetroRenderProfile &Profile, int32 Index) */
FRotator PointStarRotation(const FLevelRetroRenderProfile &Profile,
                           int32 Index) {
  return FRotationMatrix::MakeFromZ(
             -PointStarLocation(Profile, Index).GetSafeNormal())
      .Rotator();
}

/** User Story: As a profile sky pixel consumer, I need to invoke point star plane right through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FVector PointStarPlaneRight(const FLevelRetroRenderProfile &Profile, int32 Index) */
FVector PointStarPlaneRight(const FLevelRetroRenderProfile &Profile,
                            int32 Index) {
  return FRotationMatrix(PointStarRotation(Profile, Index))
      .GetScaledAxis(EAxis::X);
}

/** User Story: As a profile sky pixel consumer, I need to invoke point star plane up through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FVector PointStarPlaneUp(const FLevelRetroRenderProfile &Profile, int32 Index) */
FVector PointStarPlaneUp(const FLevelRetroRenderProfile &Profile,
                         int32 Index) {
  return FRotationMatrix(PointStarRotation(Profile, Index))
      .GetScaledAxis(EAxis::Y);
}

} // namespace

/** User Story: As a profile sky pixel consumer, I need to invoke reduce moon pixel indices through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn TArray<func::GridIndex> ReduceMoonPixelIndices( const FLevelRetroRenderProfile &Profile) */
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

/** User Story: As a profile sky pixel consumer, I need to invoke reduce moon pixel quad through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FRuntimePixelQuad ReduceMoonPixelQuad(const FLevelRetroRenderProfile &Profile, const func::GridIndex &Index) */
FRuntimePixelQuad ReduceMoonPixelQuad(const FLevelRetroRenderProfile &Profile,
                                      const func::GridIndex &Index) {
  return {ReduceMoonDiscLocation(Profile) +
              MoonPixelOffset(Profile, Index),
          MoonPixelPlaneRight(Profile), MoonPixelPlaneUp(Profile),
          MoonPixelCellWorldSize(Profile) *
              Profile.MoonPixelOverlapMultiplier,
          MoonDiscColor(Profile)};
}

/** User Story: As a profile sky pixel consumer, I need to invoke reduce moon pixel mesh buffers through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FRuntimePixelMeshBuffers ReduceMoonPixelMeshBuffers( const FLevelRetroRenderProfile &Profile) */
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

/** User Story: As a profile sky pixel consumer, I need to invoke reduce point star quad through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FRuntimePixelQuad ReducePointStarQuad(const FLevelRetroRenderProfile &Profile, int32 Index) */
FRuntimePixelQuad ReducePointStarQuad(const FLevelRetroRenderProfile &Profile,
                                      int32 Index) {
  return {PointStarLocation(Profile, Index),
          PointStarPlaneRight(Profile, Index),
          PointStarPlaneUp(Profile, Index),
          PointStarWorldSize(Profile, Index), SkyDomeStarColor(Profile)};
}

/** User Story: As a profile sky pixel consumer, I need to invoke reduce point star mesh buffers through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FRuntimePixelMeshBuffers ReducePointStarMeshBuffers( const FLevelRetroRenderProfile &Profile) */
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

/** User Story: As a profile sky pixel consumer, I need to invoke reduce pixel quad normal through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn FVector ReducePixelQuadNormal(const FRuntimePixelQuad &Quad) */
FVector ReducePixelQuadNormal(const FRuntimePixelQuad &Quad) {
  return FVector::CrossProduct(Quad.Right, Quad.Up).GetSafeNormal();
}

/** User Story: As a profile sky pixel consumer, I need to invoke append pixel quad through a stable signature so the profile sky pixel workflow remains explicit and composable. @fn void AppendPixelQuad(FRuntimePixelMeshBuffers &Buffers, const FLevelRetroRenderProfile &Profile, const FRuntimePixelQuad &Quad) */
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
