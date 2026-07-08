#include "Features/Systems/Projection/Terrain/TerrainAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FTerrainState> {
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

} // namespace

ecs::FWorld ProjectTerrain(const FProjectTerrainPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
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
