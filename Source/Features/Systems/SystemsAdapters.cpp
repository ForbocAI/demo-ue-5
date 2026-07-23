#include "Features/Systems/SystemsAdapters.h"

#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Systems/Projection/Bot/ProjectionBotAdapters.h"
#include "Features/Systems/Projection/Interaction/InteractionAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionAdapters.h"
#include "Features/Systems/Projection/Spawn/SpawnAdapters.h"
#include "Features/Systems/Projection/Terrain/ProjectionTerrainAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsAdapters {

/** User Story: As a features systems consumer, I need to invoke project player through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectPlayer(const FProjectPlayerPayload &Payload) */
ecs::FWorld ProjectPlayer(const FProjectPlayerPayload &Payload) {
  return EntitiesAdapters::ProjectPlayer({Payload.World, Payload.Player});
}

/** User Story: As a features systems consumer, I need to invoke project terrain through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload) */
ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload) {
  return SystemsProjectionTerrainAdapters::ProjectTerrain(Payload);
}

/** User Story: As a features systems consumer, I need to invoke project spawn through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectSpawn(const FProjectSpawnPayload &Payload) */
ecs::FWorld ProjectSpawn(const FProjectSpawnPayload &Payload) {
  return SystemsProjectionSpawnAdapters::ProjectSpawn(Payload);
}

/** User Story: As a features systems consumer, I need to invoke project bot stats through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload) */
ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotStats(Payload);
}

/** User Story: As a features systems consumer, I need to invoke project bot position through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload) */
ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotPosition(Payload);
}

/** User Story: As a features systems consumer, I need to invoke project bot ai through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload) */
ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotAI(Payload);
}

/** User Story: As a features systems consumer, I need to invoke project bot goal through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload) */
ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotGoal(Payload);
}

/** User Story: As a features systems consumer, I need to invoke project interaction through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectInteraction(const FProjectInteractionPayload &Payload) */
ecs::FWorld ProjectInteraction(const FProjectInteractionPayload &Payload) {
  return SystemsProjectionInteractionAdapters::ProjectInteraction(Payload);
}

/** User Story: As a features systems consumer, I need to invoke project runtime world through a stable signature so the features systems workflow remains explicit and composable. @fn ecs::FWorld ProjectRuntimeWorld(const FProjectRuntimePayload &Payload) */
ecs::FWorld
ProjectRuntimeWorld(const FProjectRuntimePayload &Payload) {
  return SystemsProjectionAdapters::ProjectRuntimeWorld(Payload);
}

} // namespace SystemsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
