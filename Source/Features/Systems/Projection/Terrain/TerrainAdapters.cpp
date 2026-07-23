#include "Features/Systems/Projection/Terrain/ProjectionTerrainAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FTerrainState> {
  /** User Story: As a systems projection terrain consumer, I need to invoke the callable value through a stable signature so the systems projection terrain workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FTerrainState &Terrain) const */
  ecs::FComponentValue operator()(const FTerrainState &Terrain) const {
    return ComponentValueMap(
        {{"TerrainLoaded", Terrain.bTerrainLoaded},
         {"TerrainSource", Terrain.TerrainSource},
         {"OrthoSource", Terrain.OrthoSource},
         {"GridSize", Terrain.GridSize},
         {"MinElevationMeters", Terrain.MinElevationMeters},
         {"MaxElevationMeters", Terrain.MaxElevationMeters}});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionTerrainAdapters {
namespace {

using ComponentsAdapters::RegisteredComponentGroups;

/**
 * @fn ecs::EntityKey TerrainEntityKey()
 * @brief Returns the stable ECS entity key for terrain projection.
 *
 * User Story: As terrain projection code, I need one stable entity key so
 * terrain components update the same ECS entity after every RTK action.
 */
ecs::EntityKey TerrainEntityKey() {
  return ComponentsAdapters::ComponentAtom("level:terrain");
}

/**
 * @fn TArray<TArray<FString>> BuildTerrainDomains()
 * @brief Builds ECS domain steps for terrain projection.
 *
 * User Story: As an ECS inspector, I need projected terrain entities tagged
 * with component, system, and projection subdomains.
 */
TArray<TArray<FString>> BuildTerrainDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Components", "Level"},
       {"Systems", "Terrain"},
       {"Systems", "Projection", "Terrain"}});
}

} // namespace

/** User Story: As a systems projection terrain consumer, I need to invoke project terrain through a stable signature so the systems projection terrain workflow remains explicit and composable. @fn ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload) */
ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          func::constant<ecs::EntityKey>(TerrainEntityKey()),
          func::constant<TArray<TArray<FString>>>(BuildTerrainDomains()),
          [](const FProjectTerrainPayload &PayloadValue) -> const FTerrainState & {
            return PayloadValue.Terrain;
          },
          RegisteredComponentGroups<FTerrainState>(
              {{"Components/Level",
                {"TerrainLoaded", "GridSize", "MinElevationMeters",
                 "MaxElevationMeters"}},
               {"Components/Data", {"TerrainSource", "OrthoSource"}}})});
}

} // namespace SystemsProjectionTerrainAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
