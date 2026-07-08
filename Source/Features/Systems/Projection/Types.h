#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/Types.h"
#include "Features/Systems/Bots/AI/Types.h"
#include "Features/Systems/Bots/Goals/Types.h"
#include "Features/Systems/Bots/Position/Types.h"
#include "Features/Systems/Bots/Stats/Types.h"
#include "Features/Systems/Interaction/Types.h"
#include "Features/Systems/Spawn/Types.h"
#include "Features/Systems/Terrain/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FRuntimeState;

/**
 * @brief RTK-style payload for projecting player slice state into ECS.
 *
 * @signature struct FProjectPlayerPayload
 *
 * User Story: As the runtime reducer, I need selected player state and the
 * current ECS world to travel through one action-shaped payload.
 */
struct FProjectPlayerPayload {
  ecs::FWorld World;
  const FPlayerState &Player;
};

/**
 * @brief RTK-style payload for projecting terrain slice state into ECS.
 *
 * @signature struct FProjectTerrainPayload
 *
 * User Story: As the runtime reducer, terrain data should be normalized into
 * component values by an adapter, not by a view.
 */
struct FProjectTerrainPayload {
  ecs::FWorld World;
  const FTerrainState &Terrain;
};

/**
 * @brief RTK-style payload for projecting spawn slice state into ECS.
 *
 * @signature struct FProjectSpawnPayload
 *
 * User Story: As spawn state changes, the reducer-owned ECS projection should
 * receive one payload that contains the prior world and authoritative slice.
 */
struct FProjectSpawnPayload {
  ecs::FWorld World;
  const FSpawnState &Spawn;
};

/**
 * @brief RTK-style payload for projecting bot stats entity-adapter state.
 *
 * @signature struct FProjectBotStatsPayload
 *
 * User Story: As bot stats are selected from the store, their ECS projection
 * should stay in the systems/bots/stats adapter boundary.
 */
struct FProjectBotStatsPayload {
  ecs::FWorld World;
  const FBotStatsComponent &Stats;
};

/**
 * @brief RTK-style payload for projecting bot position entity-adapter state.
 *
 * @signature struct FProjectBotPositionPayload
 *
 * User Story: As bot positions change, ECS spatial components should be
 * written by reducer-called adapters through one payload object.
 */
struct FProjectBotPositionPayload {
  ecs::FWorld World;
  const FBotPositionComponent &Position;
};

/**
 * @brief RTK-style payload for projecting bot AI entity-adapter state.
 *
 * @signature struct FProjectBotAIPayload
 *
 * User Story: As bot AI state is selected, behavior and patrol components
 * should be projected without borrowing logic from sibling domains.
 */
struct FProjectBotAIPayload {
  ecs::FWorld World;
  const FBotAIComponent &AI;
};

/**
 * @brief RTK-style payload for projecting bot goal entity-adapter state.
 *
 * @signature struct FProjectBotGoalPayload
 *
 * User Story: As goal queues are selected, strategic goal values should be
 * normalized through a bot-goals adapter payload.
 */
struct FProjectBotGoalPayload {
  ecs::FWorld World;
  const FBotGoalComponent &Goal;
};

/**
 * @brief RTK-style payload for projecting interaction slice state into ECS.
 *
 * @signature struct FProjectInteractionPayload
 *
 * User Story: As interaction candidates change, the runtime reducer can
 * project them into ECS without moving interaction logic into views.
 */
struct FProjectInteractionPayload {
  ecs::FWorld World;
  const FInteractionState &Interaction;
};

/**
 * @brief RTK-style payload for projecting the root runtime state into ECS.
 *
 * @signature struct FProjectRuntimePayload
 *
 * User Story: As the root reducer, I need the full post-reducer store state to
 * feed selectors before replacing FRuntimeState::Ecs.World.
 */
struct FProjectRuntimePayload {
  const FRuntimeState &State;
};

} // namespace Level
} // namespace Game
} // namespace ForbocAI
