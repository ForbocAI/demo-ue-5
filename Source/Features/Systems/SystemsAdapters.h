#pragma once

#include "Features/Systems/Projection/SystemsProjectionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeState;

namespace SystemsAdapters {

/**
 * @brief Adapter facade for projecting player payloads into ECS.
 * @signature ecs::FWorld ProjectPlayer(const FProjectPlayerPayload &Payload)
 *
 * User Story: As runtime reducer code, player projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectPlayer(const FProjectPlayerPayload &Payload);

/**
 * @brief Adapter facade for projecting terrain payloads into ECS.
 * @signature ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload)
 *
 * User Story: As runtime reducer code, terrain projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload);

/**
 * @brief Adapter facade for projecting spawn payloads into ECS.
 * @signature ecs::FWorld ProjectSpawn(const FProjectSpawnPayload &Payload)
 *
 * User Story: As runtime reducer code, spawn projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectSpawn(const FProjectSpawnPayload &Payload);

/**
 * @brief Adapter facade for projecting bot stats payloads into ECS.
 * @signature ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload)
 *
 * User Story: As runtime reducer code, bot stats projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload);

/**
 * @brief Adapter facade for projecting bot position payloads into ECS.
 * @signature ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload)
 *
 * User Story: As runtime reducer code, bot position projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload);

/**
 * @brief Adapter facade for projecting bot AI payloads into ECS.
 * @signature ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload)
 *
 * User Story: As runtime reducer code, bot AI projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload);

/**
 * @brief Adapter facade for projecting bot goal payloads into ECS.
 * @signature ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload)
 *
 * User Story: As runtime reducer code, bot goal projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload);

/**
 * @brief Adapter facade for projecting interaction payloads into ECS.
 * @signature ecs::FWorld ProjectInteraction(const FProjectInteractionPayload &Payload)
 *
 * User Story: As runtime reducer code, interaction projection stays behind an
 * adapter boundary with RTK-style payload semantics.
 */
ecs::FWorld ProjectInteraction(const FProjectInteractionPayload &Payload);

/**
 * @brief Adapter facade for projecting runtime root payloads into ECS.
 * @signature ecs::FWorld ProjectRuntimeWorld(const FProjectRuntimePayload &Payload)
 *
 * User Story: As runtime reducer code, root ECS projection consumes the
 * post-reducer store state through one RTK-style payload.
 */
ecs::FWorld ProjectRuntimeWorld(const FProjectRuntimePayload &Payload);

} // namespace SystemsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
