#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingSelectors {

const FLevelRetroRenderProfile &SelectRuntimeProfile();
TArray<FLevelRetroTextureSpec> SelectTextureCatalog();
const FLevelRetroTextureSpec &SelectTextureSpec(ELevelRetroTexture Texture);

} // namespace RenderingSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
