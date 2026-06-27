#pragma once

#include "Core/rtk.hpp"

#include "CoreMinimal.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

class UMaterialInterface;

namespace ForbocAI {
namespace Demo {
namespace Level {

namespace RenderingActions {

void ApplyRuntimeProfile();
UMaterialInterface *LoadBlockoutMaterial();
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
