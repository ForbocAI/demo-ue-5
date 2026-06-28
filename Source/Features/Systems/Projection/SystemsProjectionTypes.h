#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"
#include "Features/Systems/Bots/AI/BotAITypes.h"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Interaction/InteractionTypes.h"
#include "Features/Systems/Spawn/SpawnTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FRuntimeState;

/**
 * @brief RTK-style payload for projecting player slice state into ECS.
 *
 * @signature struct FProjectPlayerEcsPayload
 *
 * User Story: As the runtime reducer, I need selected player state and the
 * current ECS world to travel through one action-shaped payload.
 */
struct FProjectPlayerEcsPayload {
  ecs::FWorld World;
  const FPlayerState &Player;
};

/**
 * @brief RTK-style payload for projecting terrain slice state into ECS.
 *
 * @signature struct FProjectTerrainEcsPayload
 *
 * User Story: As the runtime reducer, terrain data should be normalized into
 * component values by an adapter, not by a view.
 */
struct FProjectTerrainEcsPayload {
  ecs::FWorld World;
  const FTerrainState &Terrain;
};

/**
 * @brief RTK-style payload for projecting spawn slice state into ECS.
 *
 * @signature struct FProjectSpawnEcsPayload
 *
 * User Story: As spawn state changes, the reducer-owned ECS projection should
 * receive one payload that contains the prior world and authoritative slice.
 */
struct FProjectSpawnEcsPayload {
  ecs::FWorld World;
  const FSpawnState &Spawn;
};

/**
 * @brief RTK-style payload for projecting bot stats entity-adapter state.
 *
 * @signature struct FProjectBotStatsEcsPayload
 *
 * User Story: As bot stats are selected from the store, their ECS projection
 * should stay in the systems/bots/stats adapter boundary.
 */
struct FProjectBotStatsEcsPayload {
  ecs::FWorld World;
  const FBotStatsComponent &Stats;
};

/**
 * @brief RTK-style payload for projecting bot position entity-adapter state.
 *
 * @signature struct FProjectBotPositionEcsPayload
 *
 * User Story: As bot positions change, ECS spatial components should be
 * written by reducer-called adapters through one payload object.
 */
struct FProjectBotPositionEcsPayload {
  ecs::FWorld World;
  const FBotPositionComponent &Position;
};

/**
 * @brief RTK-style payload for projecting bot AI entity-adapter state.
 *
 * @signature struct FProjectBotAIEcsPayload
 *
 * User Story: As bot AI state is selected, behavior and patrol components
 * should be projected without borrowing logic from sibling domains.
 */
struct FProjectBotAIEcsPayload {
  ecs::FWorld World;
  const FBotAIComponent &AI;
};

/**
 * @brief RTK-style payload for projecting bot goal entity-adapter state.
 *
 * @signature struct FProjectBotGoalEcsPayload
 *
 * User Story: As goal queues are selected, strategic goal values should be
 * normalized through a bot-goals adapter payload.
 */
struct FProjectBotGoalEcsPayload {
  ecs::FWorld World;
  const FBotGoalComponent &Goal;
};

/**
 * @brief RTK-style payload for projecting interaction slice state into ECS.
 *
 * @signature struct FProjectInteractionEcsPayload
 *
 * User Story: As interaction candidates change, the runtime reducer can
 * project them into ECS without moving interaction logic into views.
 */
struct FProjectInteractionEcsPayload {
  ecs::FWorld World;
  const FInteractionState &Interaction;
};

/**
 * @brief RTK-style payload for projecting the root runtime state into ECS.
 *
 * @signature struct FProjectRuntimeEcsPayload
 *
 * User Story: As the root reducer, I need the full post-reducer store state to
 * feed selectors before replacing FRuntimeState::Ecs.World.
 */
struct FProjectRuntimeEcsPayload {
  const FRuntimeState &State;
};

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
