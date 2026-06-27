#pragma once

#include "Core/ecs.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"
#include "Features/Systems/Bots/AI/BotAITypes.h"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"
#include "Features/Systems/Spawn/SpawnTypes.h"
#include "Features/Systems/Terrain/TerrainTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FRuntimeState;

namespace SystemsAdapters {

ecs::FWorld ProjectPlayer(ecs::FWorld World, const FPlayerState &Player);
ecs::FWorld ProjectTerrain(ecs::FWorld World, const FTerrainState &Terrain);
ecs::FWorld ProjectSpawn(ecs::FWorld World, const FSpawnState &Spawn);
ecs::FWorld ProjectBotStats(ecs::FWorld World,
                            const FBotStatsComponent &Stats);
ecs::FWorld ProjectBotPosition(ecs::FWorld World,
                               const FBotPositionComponent &Position);
ecs::FWorld ProjectBotAI(ecs::FWorld World, const FBotAIComponent &AI);
ecs::FWorld ProjectBotGoal(ecs::FWorld World,
                           const FBotGoalComponent &Goal);
ecs::FWorld ProjectRuntimeEcsWorld(const FRuntimeState &State);

} // namespace SystemsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
