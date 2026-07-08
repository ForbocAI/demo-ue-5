#include "Features/Systems/Rendering/Profile/Sky/Slice.h"

#include "Features/Systems/Rendering/Profile/Color/Types.h"
#include "ProceduralMeshComponent.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileSkyReducers {
namespace {

using RenderingProfileColorTypes::FProfileColorChannels;
using RenderingProfileColorTypes::ProfileLinearColor;

// --- Moon pixel grid helpers (pure computation) --------------------------

float MoonPixelGridCenter(const FLevelRetroRenderProfile &Profile) {
  return static_cast<float>(Profile.MoonPixelGridSize - 1) * 0.5f;
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
  const float Y =
      MoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
  return FVector2D(X, Y).Size() <= 1.0f;
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
  const float Y =
      MoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
  const float Radius = MoonPixelWorldDiameter(Profile) * 0.5f;
  return MoonPixelPlaneRight(Profile) * (X * Radius) +
         MoonPixelPlaneUp(Profile) * (Y * Radius);
}

// --- Point star helpers (pure computation) --------------------------------

float PointStarHash(int32 Index, float Salt) {
  return FMath::Frac(
      FMath::Sin((static_cast<float>(Index) + 1.0f) * Salt) * 43758.5453f);
}

float PointStarYawDegrees(const FLevelRetroRenderProfile &Profile,
                          int32 Index) {
  return PointStarHash(Index, Profile.PointStarYawHashSalt) *
         Profile.PointStarYawSpanDegrees;
}

float PointStarPitchDegrees(const FLevelRetroRenderProfile &Profile,
                            int32 Index) {
  return Profile.PointStarPitchMinDegrees +
         PointStarHash(Index, Profile.PointStarPitchHashSalt) *
             Profile.PointStarPitchSpanDegrees;
}

float PointStarDistance(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonDiscDistance * Profile.PointStarDistanceMultiplier;
}

FVector PointStarLocation(const FLevelRetroRenderProfile &Profile,
                          int32 Index) {
  return FRotator(PointStarPitchDegrees(Profile, Index),
                  PointStarYawDegrees(Profile, Index), 0.0f)
             .Vector() *
         PointStarDistance(Profile);
}

float PointStarWorldSize(const FLevelRetroRenderProfile &Profile,
                         int32 Index) {
  return Profile.PointStarWorldSizeMin +
         PointStarHash(Index, Profile.PointStarSizeHashSalt) *
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

// --- Profile color selectors ---------------------------------------------

FLinearColor SkyAtmosphereSkyLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceR,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceG,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceB,
                &FLevelRetroRenderProfile::SkyAtmosphereSkyLuminanceA});
}

FLinearColor SkyAtmosphereAerialLuminance(
    const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceR,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceG,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceB,
                &FLevelRetroRenderProfile::SkyAtmosphereAerialLuminanceA});
}

FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyLightColorR,
                &FLevelRetroRenderProfile::SkyLightColorG,
                &FLevelRetroRenderProfile::SkyLightColorB,
                &FLevelRetroRenderProfile::SkyLightColorA});
}

FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeStarColorR,
                &FLevelRetroRenderProfile::SkyDomeStarColorG,
                &FLevelRetroRenderProfile::SkyDomeStarColorB,
                &FLevelRetroRenderProfile::SkyDomeStarColorA});
}

FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeHorizonColorR,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorG,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorB,
                &FLevelRetroRenderProfile::SkyDomeHorizonColorA});
}

FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::SkyDomeZenithColorR,
                &FLevelRetroRenderProfile::SkyDomeZenithColorG,
                &FLevelRetroRenderProfile::SkyDomeZenithColorB,
                &FLevelRetroRenderProfile::SkyDomeZenithColorA});
}

FLinearColor SkyDomeTextureStarMaskColor(
    const FLevelRetroRenderProfile &Profile) {
  (void)Profile;
  return FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) {
  return ProfileLinearColor(
      Profile, {&FLevelRetroRenderProfile::MoonDiscColorR,
                &FLevelRetroRenderProfile::MoonDiscColorG,
                &FLevelRetroRenderProfile::MoonDiscColorB,
                &FLevelRetroRenderProfile::MoonDiscColorA});
}

// --- Sky dome reducers ---------------------------------------------------

FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile) {
  return FVector::UpVector * Profile.SkyDomeZOffset;
}

FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile) {
  return FVector(Profile.SkyDomeScale);
}

// --- Moon disc reducers --------------------------------------------------

FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile) {
  return FRotator(Profile.MoonDiscPitchDegrees, Profile.MoonDiscYawDegrees,
                  Profile.MoonDiscRollDegrees)
             .Vector() *
         Profile.MoonDiscDistance;
}

FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile) {
  const FVector Location = ReduceMoonDiscLocation(Profile);
  return FRotationMatrix::MakeFromZ(-Location.GetSafeNormal()).Rotator();
}

int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile) {
  return Profile.MoonPixelGridSize;
}

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
        AppendPixelQuad(Buffers, ReduceMoonPixelQuad(Profile, Index));
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
        AppendPixelQuad(Buffers, ReducePointStarQuad(Profile, Index));
      });
  return Buffers;
}

// --- Shared pixel mesh reducer -------------------------------------------

FVector ReducePixelQuadNormal(const FRuntimePixelQuad &Quad) {
  return FVector::CrossProduct(Quad.Right, Quad.Up).GetSafeNormal();
}

void AppendPixelQuad(FRuntimePixelMeshBuffers &Buffers,
                     const FRuntimePixelQuad &Quad) {
  const int32 BaseIndex = Buffers.Vertices.Num();
  const FVector Right = Quad.Right * (Quad.Size * 0.5f);
  const FVector Up = Quad.Up * (Quad.Size * 0.5f);
  const FVector Normal = ReducePixelQuadNormal(Quad);
  Buffers.Vertices.Append(
      {Quad.Center - Right - Up, Quad.Center + Right - Up,
       Quad.Center + Right + Up, Quad.Center - Right + Up});
  Buffers.Triangles.Append({BaseIndex, BaseIndex + 1, BaseIndex + 2,
                            BaseIndex, BaseIndex + 2, BaseIndex + 3,
                            BaseIndex + 2, BaseIndex + 1, BaseIndex,
                            BaseIndex + 3, BaseIndex + 2, BaseIndex});
  Buffers.Normals.Append({Normal, Normal, Normal, Normal});
  Buffers.UV0.Append({FVector2D(0.0f, 0.0f), FVector2D(1.0f, 0.0f),
                      FVector2D(1.0f, 1.0f), FVector2D(0.0f, 1.0f)});
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
