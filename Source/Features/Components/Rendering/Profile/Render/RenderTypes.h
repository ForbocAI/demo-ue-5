#pragma once

#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FLevelRetroRenderProfile {
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

} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Components/Rendering/Profile/Render/Equality/EqualityTypes.h"
