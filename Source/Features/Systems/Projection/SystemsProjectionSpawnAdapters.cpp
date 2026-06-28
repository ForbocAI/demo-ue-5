#include "Features/Systems/Projection/SystemsProjectionSpawnAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionSpawnAdapters {
namespace {

struct FBuildSpawnComponentStepsPayload {
  ecs::EntityKey Entity;
  const FSpawnState &Spawn;
};

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
 * @signature TArray<ComponentsAdapters::FEcsDomainProjectionStep> BuildSpawnDomainSteps(const ecs::EntityKey &Entity)
 *
 * User Story: As an ECS inspector, I need player spawn data to be visible
 * under systems/spawn, systems/projection/spawn, and player entity domains.
 */
TArray<ComponentsAdapters::FEcsDomainProjectionStep>
BuildSpawnDomainSteps(const ecs::EntityKey &Entity) {
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Steps;
  Steps.Add({Entity, {TEXT("Systems"), TEXT("Spawn")}});
  Steps.Add({Entity, {TEXT("Systems"), TEXT("Projection"), TEXT("Spawn")}});
  Steps.Add(
      {Entity, {TEXT("Entities"), TEXT("Characters"), TEXT("Player")}});
  return Steps;
}

/**
 * @brief Builds ECS component steps from spawn slice state.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionStep> BuildSpawnComponentSteps(const FBuildSpawnComponentStepsPayload &Payload)
 *
 * User Story: As the runtime reducer, spawn payload fields should be projected
 * into spatial ECS components with no view-side calculations.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionStep>
BuildSpawnComponentSteps(const FBuildSpawnComponentStepsPayload &Payload) {
  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Steps;
  Steps.Add({Payload.Entity, TEXT("Components/Spatial/Location"),
             ecs::vec3Value(Payload.Spawn.PlayerSpawn.Location)});
  Steps.Add({Payload.Entity, TEXT("Components/Spatial/Rotation"),
             ComponentsAdapters::RotationValue(
                 Payload.Spawn.PlayerSpawn.Rotation)});
  Steps.Add({Payload.Entity, TEXT("Components/Level/AnchorLabel"),
             ecs::textValue(Payload.Spawn.PlayerSpawn.AnchorLabel)});
  return Steps;
}

} // namespace

ecs::FWorld ProjectSpawn(const FProjectSpawnEcsPayload &Payload) {
  const ecs::EntityKey Entity = SpawnEntityKey();
  return (func::pipe(Payload.World) |
          [Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithDomainSteps(
                {World, BuildSpawnDomainSteps(Entity), 0});
          } |
          [&Payload, Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithComponentSteps(
                {World, BuildSpawnComponentSteps({Entity, Payload.Spawn}), 0});
          })
      .val;
}

} // namespace SystemsProjectionSpawnAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
