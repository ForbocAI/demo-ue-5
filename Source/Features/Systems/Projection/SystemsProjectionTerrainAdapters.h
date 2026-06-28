#pragma once

#include "Features/Systems/Projection/SystemsProjectionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionTerrainAdapters {

/**
 * @brief Projects terrain slice data into ECS domain and component state.
 *
 * @signature ecs::FWorld ProjectTerrain(const FProjectTerrainEcsPayload &Payload)
 *
 * User Story: As a runtime reducer, I need terrain slice state converted into
 * ECS components through a systems/terrain adapter.
 */
ecs::FWorld ProjectTerrain(const FProjectTerrainEcsPayload &Payload);

} // namespace SystemsProjectionTerrainAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
