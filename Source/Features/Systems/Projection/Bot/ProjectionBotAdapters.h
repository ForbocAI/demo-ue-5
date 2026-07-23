#pragma once

#include "Features/Systems/Projection/ProjectionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsProjectionBotAdapters {

/**
 * @fn ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload)
 * @brief Projects bot stats entity-adapter rows into ECS component state.
 *
 *
 * User Story: As a runtime reducer, I need selected bot stats rows normalized
 * into ECS through the systems/projection/bots/stats adapter.
 */
ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload);

/**
 * @fn ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload)
 * @brief Projects bot position entity-adapter rows into ECS component state.
 *
 *
 * User Story: As a runtime reducer, I need selected bot position rows
 * normalized into ECS through the systems/projection/bots/position adapter.
 */
ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload);

/**
 * @fn ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload)
 * @brief Projects bot AI entity-adapter rows into ECS component state.
 *
 *
 * User Story: As a runtime reducer, I need selected bot AI rows normalized into
 * ECS through the systems/projection/bots/ai adapter.
 */
ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload);

/**
 * @fn ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload)
 * @brief Projects bot goal entity-adapter rows into ECS component state.
 *
 *
 * User Story: As a runtime reducer, I need selected bot goal rows normalized
 * into ECS through the systems/projection/bots/goals adapter.
 */
ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload);

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
