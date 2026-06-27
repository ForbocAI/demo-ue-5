#include "Features/Systems/Spawn/SpawnSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpawnSelectors {

FSpawnPointPayload SelectPlayerSpawn(const FSpawnState &State) {
  return (func::pipe(State.PlayerSpawn) |
         [](const FSpawnPointPayload &PlayerSpawn) -> FSpawnPointPayload {
    return PlayerSpawn;
  }).val;
}

} // namespace SpawnSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
