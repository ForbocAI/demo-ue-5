#pragma once

#include "Features/Systems/Projection/ProjectionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsProjectionInteractionAdapters {

/**
 * @fn ecs::FWorld ProjectInteraction(const FProjectInteractionPayload &Payload)
 * @brief Projects interaction slice state into ECS domain and component state.
 *
 *
 * User Story: As a runtime reducer, I need selected interaction state
 * normalized into ECS without adding candidate logic to views.
 */
ecs::FWorld ProjectInteraction(const FProjectInteractionPayload &Payload);

} // namespace SystemsProjectionInteractionAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
