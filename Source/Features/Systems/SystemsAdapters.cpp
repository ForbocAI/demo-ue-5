#include "Features/Systems/SystemsAdapters.h"

#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionBotAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionInteractionAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionRuntimeAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionSpawnAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionTerrainAdapters.h"

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
