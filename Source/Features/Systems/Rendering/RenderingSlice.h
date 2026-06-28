#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

class UMaterialInterface;

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingSlice {

FRenderingState CreateInitialState();
const rtk::Slice<FRenderingState> &GetSlice();
void ApplyRuntimeProfile(const FLevelRetroRenderProfile &Profile);
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
