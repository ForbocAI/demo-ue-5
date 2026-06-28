#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Runtime/RuntimeTypes.h"

class UMaterialInterface;

namespace ForbocAI {
namespace Demo {
namespace Level {

namespace RenderingThunks {

rtk::ThunkAction<FRenderingPayload, FRuntimeState>
ObserveRuntimeProfile(const FString &Id);

void ApplyRuntimeProfile(const FLevelRetroRenderProfile &Profile);
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
