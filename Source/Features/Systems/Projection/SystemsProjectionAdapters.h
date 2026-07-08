#pragma once

#include "Features/Systems/Projection/ProjectionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsProjectionAdapters {

/**
 * @brief Projects post-reducer runtime root state into a fresh ECS world.
 *
 * @signature ecs::FWorld ProjectRuntimeWorld(const FProjectRuntimePayload &Payload)
 *
 * User Story: As the runtime root reducer, I need to derive the ECS world from
 * the latest single-store state using selectors and adapter-owned projection.
 */
ecs::FWorld ProjectRuntimeWorld(const FProjectRuntimePayload &Payload);

} // namespace SystemsProjectionAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
