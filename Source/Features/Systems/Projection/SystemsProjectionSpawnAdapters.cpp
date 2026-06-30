#include "Features/Systems/Projection/SystemsProjectionSpawnAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionSpawnAdapters {
namespace {

/**
 * @brief Returns the stable ECS entity key for player spawn projection.
 * @signature ecs::EntityKey SpawnEntityKey()
 *
 * User Story: As spawn projection code, I need one stable entity key so
 * reducers update the player-spawn ECS record consistently.
 */
ecs::EntityKey SpawnEntityKey() { return TEXT("level:spawn:player"); }

/**
 * @brief Builds ECS domain steps for player spawn projection.
 * @signature TArray<TArray<FString>> BuildSpawnDomains()
 *
 * User Story: As an ECS inspector, I need player spawn data to be visible
 * under systems/spawn, systems/projection/spawn, and player entity domains.
 */
TArray<TArray<FString>> BuildSpawnDomains() {
  return {{TEXT("Systems"), TEXT("Spawn")},
          {TEXT("Systems"), TEXT("Projection"), TEXT("Spawn")},
          {TEXT("Entities"), TEXT("Characters"), TEXT("Player")}};
}

/**
 * @brief Builds ECS component steps from spawn slice state.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionBinding> BuildSpawnComponentBindings(const FSpawnState &Spawn)
 *
 * User Story: As the runtime reducer, spawn payload fields should be projected
 * into spatial ECS components with no view-side calculations.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
BuildSpawnComponentBindings(const FSpawnState &Spawn) {
  return {{TEXT("Components/Spatial/Location"),
           ecs::vec3Value(Spawn.PlayerSpawn.Location)},
          {TEXT("Components/Spatial/Rotation"),
           ComponentsAdapters::RotationValue(Spawn.PlayerSpawn.Rotation)},
          {TEXT("Components/Level/AnchorLabel"),
           ecs::textValue(Spawn.PlayerSpawn.AnchorLabel)}};
}

} // namespace

ecs::FWorld ProjectSpawn(const FProjectSpawnEcsPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectSpawnEcsPayload &) { return SpawnEntityKey(); },
      [](const FProjectSpawnEcsPayload &) { return BuildSpawnDomains(); },
      [](const FProjectSpawnEcsPayload &PayloadValue) {
        return BuildSpawnComponentBindings(PayloadValue.Spawn);
      });
}

} // namespace SystemsProjectionSpawnAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
