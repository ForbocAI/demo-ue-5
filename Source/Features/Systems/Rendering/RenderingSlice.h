#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingActions.h"
#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Rendering/RenderingSelectors.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingSlice {

inline void ApplyRuntimeProfile() { RenderingActions::ApplyRuntimeProfile(); }

inline UMaterialInterface *LoadBlockoutMaterial() {
  return RenderingActions::LoadBlockoutMaterial();
}

inline void ApplyTexture(const FLevelRetroTextureApply &Request) {
  RenderingActions::ApplyTexture(Request);
}

} // namespace RenderingSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
