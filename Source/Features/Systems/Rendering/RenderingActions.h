#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingActions {

const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved();

} // namespace RenderingActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
