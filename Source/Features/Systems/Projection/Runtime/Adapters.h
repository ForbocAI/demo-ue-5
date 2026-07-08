#pragma once

#include "Features/Systems/Projection/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsProjectionRuntimeAdapters {

/**
 * @brief Projects post-reducer runtime root state into a fresh ECS world.
 *
 * @signature ecs::FWorld ProjectRuntimeWorld(const FProjectRuntimePayload &Payload)
 *
 * User Story: As the runtime root reducer, I need to derive the ECS world from
 * the latest single-store state using selectors and adapter-owned projection.
 */
ecs::FWorld ProjectRuntimeWorld(const FProjectRuntimePayload &Payload);

} // namespace SystemsProjectionRuntimeAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
