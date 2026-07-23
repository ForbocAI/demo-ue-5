#pragma once

#include "CoreMinimal.h"

#include "Core/fp.hpp"
#include "Features/Components/Rendering/Distance/Lod/ComponentsRenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"
#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

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

/** User Story: As a rendering profile sky consumer, I need to invoke sky atmosphere sky luminance through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FLinearColor SkyAtmosphereSkyLuminance( const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyAtmosphereSkyLuminance(
    const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke sky atmosphere aerial luminance through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FLinearColor SkyAtmosphereAerialLuminance( const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyAtmosphereAerialLuminance(
    const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke sky light color through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyLightColor(const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke sky dome star color through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeStarColor(const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke sky dome horizon color through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeHorizonColor(const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke sky dome zenith color through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeZenithColor(const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke sky dome texture star mask color through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FLinearColor SkyDomeTextureStarMaskColor( const FLevelRetroRenderProfile &Profile) */
FLinearColor SkyDomeTextureStarMaskColor(
    const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke moon disc color through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile) */
FLinearColor MoonDiscColor(const FLevelRetroRenderProfile &Profile);

// --- Sky dome reducers (pure geometry from profile) ----------------------

/** User Story: As a rendering profile sky consumer, I need to invoke reduce sky dome location through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile) */
FVector ReduceSkyDomeLocation(const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke reduce sky dome scale through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile) */
FVector ReduceSkyDomeScale(const FLevelRetroRenderProfile &Profile);

// --- Moon disc reducers (pure geometry from profile) ---------------------

/** User Story: As a rendering profile sky consumer, I need to invoke reduce moon disc location through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile) */
FVector ReduceMoonDiscLocation(const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke reduce moon disc rotation through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile) */
FRotator ReduceMoonDiscRotation(const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke reduce moon pixel grid size through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile) */
int32 ReduceMoonPixelGridSize(const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke reduce moon pixel indices through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn TArray<func::GridIndex> ReduceMoonPixelIndices( const FLevelRetroRenderProfile &Profile) */
TArray<func::GridIndex> ReduceMoonPixelIndices(
    const FLevelRetroRenderProfile &Profile);
/** User Story: As a rendering profile sky consumer, I need to invoke reduce moon pixel quad through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FRuntimePixelQuad ReduceMoonPixelQuad(const FLevelRetroRenderProfile &Profile, const func::GridIndex &Index) */
FRuntimePixelQuad ReduceMoonPixelQuad(const FLevelRetroRenderProfile &Profile,
                                      const func::GridIndex &Index);
/** User Story: As a rendering profile sky consumer, I need to invoke reduce moon pixel mesh buffers through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FRuntimePixelMeshBuffers ReduceMoonPixelMeshBuffers( const FLevelRetroRenderProfile &Profile) */
FRuntimePixelMeshBuffers ReduceMoonPixelMeshBuffers(
    const FLevelRetroRenderProfile &Profile);

// --- Point stars reducers (pure geometry from profile) -------------------

/** User Story: As a rendering profile sky consumer, I need to invoke reduce point star quad through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FRuntimePixelQuad ReducePointStarQuad(const FLevelRetroRenderProfile &Profile, int32 Index) */
FRuntimePixelQuad ReducePointStarQuad(const FLevelRetroRenderProfile &Profile,
                                      int32 Index);
/** User Story: As a rendering profile sky consumer, I need to invoke reduce point star mesh buffers through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FRuntimePixelMeshBuffers ReducePointStarMeshBuffers( const FLevelRetroRenderProfile &Profile) */
FRuntimePixelMeshBuffers ReducePointStarMeshBuffers(
    const FLevelRetroRenderProfile &Profile);

// --- Shared pixel mesh reducer ------------------------------------------

/** User Story: As a rendering profile sky consumer, I need to invoke reduce pixel quad normal through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn FVector ReducePixelQuadNormal(const FRuntimePixelQuad &Quad) */
FVector ReducePixelQuadNormal(const FRuntimePixelQuad &Quad);
/** User Story: As a rendering profile sky consumer, I need to invoke append pixel quad through a stable signature so the rendering profile sky workflow remains explicit and composable. @fn void AppendPixelQuad(FRuntimePixelMeshBuffers &Buffers, const FLevelRetroRenderProfile &Profile, const FRuntimePixelQuad &Quad) */
void AppendPixelQuad(FRuntimePixelMeshBuffers &Buffers,
                     const FLevelRetroRenderProfile &Profile,
                     const FRuntimePixelQuad &Quad);

} // namespace RenderingProfileSkyReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
