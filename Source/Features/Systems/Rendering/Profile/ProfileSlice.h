#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileReducers {

/**
 * @brief Maps JSON-backed runtime rendering settings into RTK state.
 *
 * @signature FLevelRetroRenderProfile ReduceRuntimeProfile(const
 * ForbocAI::Game::Data::FRenderingProfileSettings &Settings)
 *
 * User story: As a runtime operator, runtime rendering quality can be tuned from
 * data while the rendering slice remains the owner of store semantics.
 */
FLevelRetroRenderProfile ReduceRuntimeProfile(
    const ForbocAI::Game::Data::FRenderingProfileSettings &Settings);

} // namespace RenderingProfileReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
