#include "Features/Systems/Spawn/SpawnReducers.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnReducers {

FSpawnState
ReducePlayerSpawnAnchored(const FSpawnState &State,
                          const rtk::PayloadAction<FSpawnPointPayload> &Action) {
  return (func::pipe(State) | [&](FSpawnState Next) -> FSpawnState {
  Next.PlayerSpawn = Action.PayloadValue;
  return Next;
  }).val;
}

} // namespace SpawnReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
