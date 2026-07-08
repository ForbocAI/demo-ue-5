#pragma once

#include "Features/Systems/Rendering/Profile/Sky/SkySlice.h"

#include "Features/Systems/Rendering/Profile/Color/ColorTypes.h"
#include "ProceduralMeshComponent.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileSkyReducers {
namespace {

using RenderingProfileColorTypes::FChannels;
using RenderingProfileColorTypes::ProfileLinearColor;

// --- Moon pixel grid helpers (pure computation) --------------------------

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
  const float Y =
      MoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
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
  const float Y =
      MoonPixelCoordinate(Profile, static_cast<int32>(Index.Row));
  const float Radius =
      MoonPixelWorldDiameter(Profile) * Profile.PixelQuadHalfExtentMultiplier;
  return MoonPixelPlaneRight(Profile) * (X * Radius) +
         MoonPixelPlaneUp(Profile) * (Y * Radius);
}

// --- Point star helpers (pure computation) --------------------------------

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
} // namespace RenderingProfileSkyReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
