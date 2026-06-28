#pragma once

#include "Features/Systems/Projection/SystemsProjectionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FRuntimeState;

namespace SystemsAdapters {

/**
 * @brief Adapter facade for projecting player payloads into ECS.
 * @signature ecs::FWorld ProjectPlayer(const FProjectPlayerEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, player projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectPlayer(const FProjectPlayerEcsPayload &Payload);

/**
 * @brief Adapter facade for projecting terrain payloads into ECS.
 * @signature ecs::FWorld ProjectTerrain(const FProjectTerrainEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, terrain projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectTerrain(const FProjectTerrainEcsPayload &Payload);

/**
 * @brief Adapter facade for projecting spawn payloads into ECS.
 * @signature ecs::FWorld ProjectSpawn(const FProjectSpawnEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, spawn projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectSpawn(const FProjectSpawnEcsPayload &Payload);

/**
 * @brief Adapter facade for projecting bot stats payloads into ECS.
 * @signature ecs::FWorld ProjectBotStats(const FProjectBotStatsEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, bot stats projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectBotStats(const FProjectBotStatsEcsPayload &Payload);

/**
 * @brief Adapter facade for projecting bot position payloads into ECS.
 * @signature ecs::FWorld ProjectBotPosition(const FProjectBotPositionEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, bot position projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectBotPosition(const FProjectBotPositionEcsPayload &Payload);

/**
 * @brief Adapter facade for projecting bot AI payloads into ECS.
 * @signature ecs::FWorld ProjectBotAI(const FProjectBotAIEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, bot AI projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectBotAI(const FProjectBotAIEcsPayload &Payload);

/**
 * @brief Adapter facade for projecting bot goal payloads into ECS.
 * @signature ecs::FWorld ProjectBotGoal(const FProjectBotGoalEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, bot goal projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectBotGoal(const FProjectBotGoalEcsPayload &Payload);

/**
 * @brief Adapter facade for projecting interaction payloads into ECS.
 * @signature ecs::FWorld ProjectInteraction(const FProjectInteractionEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, interaction projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectInteraction(const FProjectInteractionEcsPayload &Payload);

/**
 * @brief Adapter facade for projecting runtime root payloads into ECS.
 * @signature ecs::FWorld ProjectRuntimeEcsWorld(const FProjectRuntimeEcsPayload &Payload)
 *
 * User Story: As runtime reducer code, root ECS projection consumes the
 * post-reducer store state through one RTK-style payload.
 */
ecs::FWorld ProjectRuntimeEcsWorld(const FProjectRuntimeEcsPayload &Payload);

} // namespace SystemsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
