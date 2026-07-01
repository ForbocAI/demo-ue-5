#pragma once

#include "Features/Systems/Projection/SystemsProjectionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionBotAdapters {

/**
 * @brief Projects bot stats entity-adapter rows into ECS component state.
 *
 * @signature ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload)
 *
 * User Story: As a runtime reducer, I need selected bot stats rows normalized
 * into ECS through the systems/projection/bots/stats adapter.
 */
ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload);

/**
 * @brief Projects bot position entity-adapter rows into ECS component state.
 *
 * @signature ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload)
 *
 * User Story: As a runtime reducer, I need selected bot position rows
 * normalized into ECS through the systems/projection/bots/position adapter.
 */
ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload);

/**
 * @brief Projects bot AI entity-adapter rows into ECS component state.
 *
 * @signature ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload)
 *
 * User Story: As a runtime reducer, I need selected bot AI rows normalized into
 * ECS through the systems/projection/bots/ai adapter.
 */
ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload);

/**
 * @brief Projects bot goal entity-adapter rows into ECS component state.
 *
 * @signature ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload)
 *
 * User Story: As a runtime reducer, I need selected bot goal rows normalized
 * into ECS through the systems/projection/bots/goals adapter.
 */
ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload);

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
