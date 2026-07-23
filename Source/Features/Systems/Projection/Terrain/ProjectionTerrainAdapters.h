#pragma once

#include "Features/Systems/Projection/ProjectionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsProjectionTerrainAdapters {

/**
 * @fn ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload)
 * @brief Projects terrain slice data into ECS domain and component state.
 *
 *
 * User Story: As a runtime reducer, I need terrain slice state converted into
 * ECS components through a systems/terrain adapter.
 */
ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload);

} // namespace SystemsProjectionTerrainAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
