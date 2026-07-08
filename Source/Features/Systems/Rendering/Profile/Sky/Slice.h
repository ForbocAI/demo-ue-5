#pragma once

#include "CoreMinimal.h"

#include "Core/ue_fp.hpp"
#include "Features/Components/Rendering/Distance/Lod/Types.h"
#include "Features/Components/Rendering/Texture/Types.h"
#include "Features/Components/Rendering/Profile/Types.h"

class UProceduralMeshComponent;
struct FProcMeshTangent;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileSkyReducers {

// --- Pixel mesh value types (shared by moon disc + point stars) -----------

struct FRuntimePixelQuad {
  FVector Center;
  FVector Right;
  FVector Up;
  float Size;
  FLinearColor Color;
};

struct FRuntimePixelMeshBuffers {
  TArray<FVector> Vertices;
  TArray<int32> Triangles;
  TArray<FVector> Normals;
  TArray<FVector2D> UV0;
  TArray<FLinearColor> VertexColors;
  TArray<FProcMeshTangent> Tangents;
};

// --- Profile color selectors (pure reads from profile state) -------------

FLinearColor SkyAtmosphereSkyLuminance(
    const FLevelRetroRenderProfile &Profile);
FLinearColor SkyAtmosphereAerialLuminance(
    const FLevelRetroRenderProfile &Profile);
FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile);
FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile);
FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile);
FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile);
FLinearColor SkyDomeTextureStarMaskColor(
    const FLevelRetroRenderProfile &Profile);
FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile);

// --- Sky dome reducers (pure geometry from profile) ----------------------

FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile);
FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile);

// --- Moon disc reducers (pure geometry from profile) ---------------------

FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile);
FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile);
int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile);
TArray<func::GridIndex> ReduceMoonPixelIndices(
    const FLevelRetroRenderProfile &Profile);
FRuntimePixelQuad ReduceMoonPixelQuad(const FLevelRetroRenderProfile &Profile,
                                      const func::GridIndex &Index);
FRuntimePixelMeshBuffers ReduceMoonPixelMeshBuffers(
    const FLevelRetroRenderProfile &Profile);

// --- Point stars reducers (pure geometry from profile) -------------------

FRuntimePixelQuad ReducePointStarQuad(const FLevelRetroRenderProfile &Profile,
                                      int32 Index);
FRuntimePixelMeshBuffers ReducePointStarMeshBuffers(
    const FLevelRetroRenderProfile &Profile);

// --- Shared pixel mesh reducer ------------------------------------------

FVector ReducePixelQuadNormal(const FRuntimePixelQuad &Quad);
void AppendPixelQuad(FRuntimePixelMeshBuffers &Buffers,
                     const FRuntimePixelQuad &Quad);

} // namespace RenderingProfileSkyReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
