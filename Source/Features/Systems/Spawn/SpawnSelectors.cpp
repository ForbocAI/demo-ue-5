#include "Features/Systems/Spawn/SpawnSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace SpawnSelectors {

FSpawnPointPayload SelectPlayerSpawn(const FSpawnState &State) {
  return State.PlayerSpawn;
}

} // namespace SpawnSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
