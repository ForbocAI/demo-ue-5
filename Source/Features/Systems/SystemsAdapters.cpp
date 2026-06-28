#include "Features/Systems/SystemsAdapters.h"

#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionBotAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionInteractionAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionRuntimeAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionSpawnAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionTerrainAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsAdapters {

ecs::FWorld ProjectPlayer(const FProjectPlayerEcsPayload &Payload) {
  return EntitiesAdapters::ProjectPlayer({Payload.World, Payload.Player});
}

ecs::FWorld ProjectTerrain(const FProjectTerrainEcsPayload &Payload) {
  return SystemsProjectionTerrainAdapters::ProjectTerrain(Payload);
}

ecs::FWorld ProjectSpawn(const FProjectSpawnEcsPayload &Payload) {
  return SystemsProjectionSpawnAdapters::ProjectSpawn(Payload);
}

ecs::FWorld ProjectBotStats(const FProjectBotStatsEcsPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotStats(Payload);
}

ecs::FWorld ProjectBotPosition(const FProjectBotPositionEcsPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotPosition(Payload);
}

ecs::FWorld ProjectBotAI(const FProjectBotAIEcsPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotAI(Payload);
}

ecs::FWorld ProjectBotGoal(const FProjectBotGoalEcsPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotGoal(Payload);
}

ecs::FWorld ProjectInteraction(const FProjectInteractionEcsPayload &Payload) {
  return SystemsProjectionInteractionAdapters::ProjectInteraction(Payload);
}

ecs::FWorld
ProjectRuntimeEcsWorld(const FProjectRuntimeEcsPayload &Payload) {
  return SystemsProjectionRuntimeAdapters::ProjectRuntimeEcsWorld(Payload);
}

} // namespace SystemsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
