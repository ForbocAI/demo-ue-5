#include "Features/Systems/Projection/SystemsProjectionTerrainAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionTerrainAdapters {
namespace {

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
 * @signature TArray<TArray<FString>> BuildTerrainDomains()
 *
 * User Story: As an ECS inspector, I need projected terrain entities tagged
 * with component, system, and projection subdomains.
 */
TArray<TArray<FString>> BuildTerrainDomains() {
  return {{TEXT("Components"), TEXT("Level")},
          {TEXT("Systems"), TEXT("Terrain")},
          {TEXT("Systems"), TEXT("Projection"), TEXT("Terrain")}};
}

/**
 * @brief Builds ECS component steps from terrain slice state.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionBinding> BuildTerrainComponentBindings(const FTerrainState &Terrain)
 *
 * User Story: As the runtime reducer, terrain slice fields should become
 * normalized ECS components through a declarative component-step list.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
BuildTerrainComponentBindings(const FTerrainState &Terrain) {
  return {{TEXT("Components/Level/TerrainLoaded"),
           ecs::boolValue(Terrain.bTerrainLoaded)},
          {TEXT("Components/Data/TerrainSource"),
           ecs::textValue(Terrain.TerrainSource)},
          {TEXT("Components/Data/OrthoSource"),
           ecs::textValue(Terrain.OrthoSource)},
          {TEXT("Components/Level/GridSize"), ecs::intValue(Terrain.GridSize)},
          {TEXT("Components/Level/MinElevationMeters"),
           ecs::floatValue(Terrain.MinElevationMeters)},
          {TEXT("Components/Level/MaxElevationMeters"),
           ecs::floatValue(Terrain.MaxElevationMeters)}};
}

} // namespace

ecs::FWorld ProjectTerrain(const FProjectTerrainEcsPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectTerrainEcsPayload &) { return TerrainEntityKey(); },
      [](const FProjectTerrainEcsPayload &) { return BuildTerrainDomains(); },
      [](const FProjectTerrainEcsPayload &PayloadValue) {
        return BuildTerrainComponentBindings(PayloadValue.Terrain);
      });
}

} // namespace SystemsProjectionTerrainAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
