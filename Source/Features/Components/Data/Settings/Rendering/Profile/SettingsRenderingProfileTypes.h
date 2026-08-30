#pragma once

#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRenderingAssetPathSettings {
  FString LevelCubeMeshPath;
  FString BlockoutMaterialPath;
  FString TerrainVertexColorMaterialPath;
};

struct FProfileSettings {
  RenderingProfile::FTime Time;
  RenderingProfile::FOutput Output;
  RenderingProfile::FScalability Scalability;
  RenderingProfile::FLighting Lighting;
  RenderingProfile::FPostProcess PostProcess;
  RenderingProfile::FSky Sky;
  RenderingProfile::FMaterial Material;
  RenderingProfile::FQuad PixelQuad;
  RenderingProfile::FFog Fog;
};

/** User Story: As the rendering profile reducer, I need the composed profile compared structurally so identical authored settings preserve the current state value. @fn inline bool operator==(const FProfileSettings &Left, const FProfileSettings &Right) */
inline bool operator==(const FProfileSettings &Left,
                       const FProfileSettings &Right) {
  return Left.Time == Right.Time && Left.Output == Right.Output &&
         Left.Scalability == Right.Scalability &&
         Left.Lighting == Right.Lighting &&
         Left.PostProcess == Right.PostProcess && Left.Sky == Right.Sky &&
         Left.Material == Right.Material && Left.PixelQuad == Right.PixelQuad &&
         Left.Fog == Right.Fog;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
