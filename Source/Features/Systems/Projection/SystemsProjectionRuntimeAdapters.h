#pragma once

#include "Features/Systems/Projection/SystemsProjectionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionRuntimeAdapters {

/**
 * @brief Projects post-reducer runtime root state into a fresh ECS world.
 *
 * @signature ecs::FWorld ProjectRuntimeEcsWorld(const FProjectRuntimeEcsPayload &Payload)
 *
 * User Story: As the runtime root reducer, I need to derive the ECS world from
 * the latest single-store state using selectors and adapter-owned projection.
 */
ecs::FWorld ProjectRuntimeEcsWorld(const FProjectRuntimeEcsPayload &Payload);

} // namespace SystemsProjectionRuntimeAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
