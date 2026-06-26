#include "Features/Systems/Spawn/SpawnReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace SpawnReducers {

FSpawnState
ReducePlayerSpawnAnchored(const FSpawnState &State,
                          const rtk::Action<FSpawnPointPayload> &Action) {
  FSpawnState Next = State;
  Next.PlayerSpawn = Action.PayloadValue;
  return Next;
}

} // namespace SpawnReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
