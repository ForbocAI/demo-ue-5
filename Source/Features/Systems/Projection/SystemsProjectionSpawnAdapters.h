#pragma once

#include "Features/Systems/Projection/SystemsProjectionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionSpawnAdapters {

/**
 * @brief Projects spawn slice data into ECS domain and component state.
 *
 * @signature ecs::FWorld ProjectSpawn(const FProjectSpawnEcsPayload &Payload)
 *
 * User Story: As a runtime reducer, I need player spawn state normalized into
 * ECS through a systems/spawn adapter boundary.
 */
ecs::FWorld ProjectSpawn(const FProjectSpawnEcsPayload &Payload);

} // namespace SystemsProjectionSpawnAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
