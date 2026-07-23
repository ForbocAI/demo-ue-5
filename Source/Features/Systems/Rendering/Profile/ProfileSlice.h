#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingProfileReducers {

/**
 * @fn FLevelRetroRenderProfile ReduceRuntimeProfile( const ForbocAI::Game::Data::FProfileSettings &Settings)
 * @brief Maps JSON-backed runtime rendering settings into RTK state.
 *
 *
 * User story: As a runtime operator, runtime rendering quality can be tuned from
 * data while the rendering slice remains the owner of store semantics.
 * User Story: As a systems rendering profile consumer, I need to invoke reduce runtime profile through a stable signature so the systems rendering profile workflow remains explicit and composable.
 */
FLevelRetroRenderProfile ReduceRuntimeProfile(
    const ForbocAI::Game::Data::FProfileSettings &Settings);

} // namespace RenderingProfileReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
