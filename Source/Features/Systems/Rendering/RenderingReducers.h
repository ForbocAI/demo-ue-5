#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingReducers {

FLevelRetroTextureApply
NormalizeTextureApply(const FLevelRetroTextureApply &Input);

FRenderingState
ReduceRenderingProfileObserved(const FRenderingState &State,
                               const rtk::PayloadAction<FRenderingPayload>
                                   &Action);

} // namespace RenderingReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
