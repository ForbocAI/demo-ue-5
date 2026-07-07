#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingActions {

const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved();
const rtk::ActionCreator<FRuntimeStatsSamplePayload> &RuntimeStatsSampled();

} // namespace RenderingActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
