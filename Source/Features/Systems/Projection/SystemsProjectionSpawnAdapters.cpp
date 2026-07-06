#include "Features/Systems/Projection/SystemsProjectionSpawnAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FSpawnPointPayload> {
  ecs::FComponentValue operator()(const FSpawnPointPayload &Spawn) const {
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

} // namespace

ecs::FWorld ProjectSpawn(const FProjectSpawnPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      func::constant<ecs::EntityKey>(SpawnEntityKey()),
      func::constant<TArray<TArray<FString>>>(BuildSpawnDomains()),
      [](const FProjectSpawnPayload &PayloadValue)
          -> const FSpawnPointPayload & {
        return PayloadValue.Spawn.PlayerSpawn;
      },
      RegisteredComponentGroups<FSpawnPointPayload>(
          {{"Components/Spatial", {"Location", "Rotation"}},
           {"Components/Level", {"AnchorLabel"}}}));
}

} // namespace SystemsProjectionSpawnAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
