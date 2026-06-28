#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RenderingActions {

const rtk::ActionCreator<FRenderingPayload> &RenderingProfileObserved();
const rtk::ActionCreator<FRenderingPresentationRequest> &
TownspersonPresentationRequested();
const rtk::ActionCreator<FRenderingPresentationRequest> &
HorsePresentationRequested();

} // namespace RenderingActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
