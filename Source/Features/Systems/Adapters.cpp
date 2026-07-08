#include "Features/Systems/Adapters.h"

#include "Features/Entities/Adapters.h"
#include "Features/Systems/Projection/Bot/Adapters.h"
#include "Features/Systems/Projection/Interaction/Adapters.h"
#include "Features/Systems/Projection/Runtime/Adapters.h"
#include "Features/Systems/Projection/Spawn/Adapters.h"
#include "Features/Systems/Projection/Terrain/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsAdapters {

ecs::FWorld ProjectPlayer(const FProjectPlayerPayload &Payload) {
  return EntitiesAdapters::ProjectPlayer({Payload.World, Payload.Player});
}

ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload) {
  return SystemsProjectionTerrainAdapters::ProjectTerrain(Payload);
}

ecs::FWorld ProjectSpawn(const FProjectSpawnPayload &Payload) {
  return SystemsProjectionSpawnAdapters::ProjectSpawn(Payload);
}

ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotStats(Payload);
}

ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotPosition(Payload);
}

ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotAI(Payload);
}

ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotGoal(Payload);
}

ecs::FWorld ProjectInteraction(const FProjectInteractionPayload &Payload) {
  return SystemsProjectionInteractionAdapters::ProjectInteraction(Payload);
}

ecs::FWorld
ProjectRuntimeWorld(const FProjectRuntimePayload &Payload) {
  return SystemsProjectionRuntimeAdapters::ProjectRuntimeWorld(Payload);
}

} // namespace SystemsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
