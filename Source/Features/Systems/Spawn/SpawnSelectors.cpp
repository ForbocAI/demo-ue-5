#include "Features/Systems/Spawn/SpawnSelectors.h"

namespace ForbocAI {
namespace Game {
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
} // namespace Game
} // namespace ForbocAI
