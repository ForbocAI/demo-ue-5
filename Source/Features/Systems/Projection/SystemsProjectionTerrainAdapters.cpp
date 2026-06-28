#include "Features/Systems/Projection/SystemsProjectionTerrainAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionTerrainAdapters {
namespace {

struct FBuildTerrainComponentStepsPayload {
  ecs::EntityKey Entity;
  const FTerrainState &Terrain;
};

/**
 * @brief Returns the stable ECS entity key for terrain projection.
 * @signature ecs::EntityKey TerrainEntityKey()
 *
 * User Story: As terrain projection code, I need one stable entity key so
 * terrain components update the same ECS entity after every RTK action.
 */
ecs::EntityKey TerrainEntityKey() { return TEXT("level:terrain"); }

/**
 * @brief Builds ECS domain steps for terrain projection.
 * @signature TArray<ComponentsAdapters::FEcsDomainProjectionStep> BuildTerrainDomainSteps(const ecs::EntityKey &Entity)
 *
 * User Story: As an ECS inspector, I need projected terrain entities tagged
 * with component, system, and projection subdomains.
 */
TArray<ComponentsAdapters::FEcsDomainProjectionStep>
BuildTerrainDomainSteps(const ecs::EntityKey &Entity) {
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Steps;
  Steps.Add({Entity, {TEXT("Components"), TEXT("Level")}});
  Steps.Add({Entity, {TEXT("Systems"), TEXT("Terrain")}});
  Steps.Add({Entity, {TEXT("Systems"), TEXT("Projection"), TEXT("Terrain")}});
  return Steps;
}

/**
 * @brief Builds ECS component steps from terrain slice state.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionStep> BuildTerrainComponentSteps(const FBuildTerrainComponentStepsPayload &Payload)
 *
 * User Story: As the runtime reducer, terrain slice fields should become
 * normalized ECS components through a declarative component-step list.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionStep>
BuildTerrainComponentSteps(const FBuildTerrainComponentStepsPayload &Payload) {
  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Steps;
  Steps.Add({Payload.Entity, TEXT("Components/Level/TerrainLoaded"),
             ecs::boolValue(Payload.Terrain.bTerrainLoaded)});
  Steps.Add({Payload.Entity, TEXT("Components/Data/TerrainSource"),
             ecs::textValue(Payload.Terrain.TerrainSource)});
  Steps.Add({Payload.Entity, TEXT("Components/Data/OrthoSource"),
             ecs::textValue(Payload.Terrain.OrthoSource)});
  Steps.Add({Payload.Entity, TEXT("Components/Level/GridSize"),
             ecs::intValue(Payload.Terrain.GridSize)});
  Steps.Add({Payload.Entity, TEXT("Components/Level/MinElevationMeters"),
             ecs::floatValue(Payload.Terrain.MinElevationMeters)});
  Steps.Add({Payload.Entity, TEXT("Components/Level/MaxElevationMeters"),
             ecs::floatValue(Payload.Terrain.MaxElevationMeters)});
  return Steps;
}

} // namespace

ecs::FWorld ProjectTerrain(const FProjectTerrainEcsPayload &Payload) {
  const ecs::EntityKey Entity = TerrainEntityKey();
  return (func::pipe(Payload.World) |
          [Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithDomainSteps(
                {World, BuildTerrainDomainSteps(Entity)});
          } |
          [&Payload, Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithComponentSteps(
                {World,
                 BuildTerrainComponentSteps({Entity, Payload.Terrain})});
          })
      .val;
}

} // namespace SystemsProjectionTerrainAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
