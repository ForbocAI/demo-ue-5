#include "Features/Systems/Projection/Spawn/SpawnAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FPointPayload> {
  ecs::FComponentValue operator()(const FPointPayload &Spawn) const {
    return ComponentValueMap({{"Location", Spawn.Location},
                              {"Rotation", Spawn.Rotation},
                              {"AnchorLabel", Spawn.AnchorLabel}});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionSpawnAdapters {
namespace {

using ComponentsAdapters::RegisteredComponentGroups;

/**
 * @brief Returns the stable ECS entity key for player spawn projection.
 * @signature ecs::EntityKey SpawnEntityKey()
 *
 * User Story: As spawn projection code, I need one stable entity key so
 * reducers update the player-spawn ECS record consistently.
 */
ecs::EntityKey SpawnEntityKey() {
  return ComponentsAdapters::ComponentAtom("level:spawn:player");
}

/**
 * @brief Builds ECS domain steps for player spawn projection.
 * @signature TArray<TArray<FString>> BuildSpawnDomains()
 *
 * User Story: As an ECS inspector, I need player spawn data to be visible
 * under systems/spawn, systems/projection/spawn, and player entity domains.
 */
TArray<TArray<FString>> BuildSpawnDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Systems", "Spawn"},
       {"Systems", "Projection", "Spawn"},
       {"Entities", "Characters", "Player"}});
}

} // namespace

ecs::FWorld ProjectSpawn(const FProjectSpawnPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          func::constant<ecs::EntityKey>(SpawnEntityKey()),
          func::constant<TArray<TArray<FString>>>(BuildSpawnDomains()),
          [](const FProjectSpawnPayload &PayloadValue)
              -> const FPointPayload & {
            return PayloadValue.Spawn.PlayerSpawn;
          },
          RegisteredComponentGroups<FPointPayload>(
              {{"Components/Spatial", {"Location", "Rotation"}},
               {"Components/Level", {"AnchorLabel"}}})});
}

} // namespace SystemsProjectionSpawnAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
