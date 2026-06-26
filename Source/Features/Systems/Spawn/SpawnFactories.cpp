#include "Features/Systems/Spawn/SpawnFactories.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace SpawnFactories {

FSpawnState CreateInitialState() {
  FSpawnState State;
  State.PlayerSpawn =
      SpawnPointPayload({FVector::ZeroVector, FRotator::ZeroRotator,
                         TEXT("unassigned")});
  return State;
}

FSpawnPointPayload SpawnPointPayload(const FSpawnPointSource &Source) {
  FSpawnPointPayload Payload;
  Payload.Location = Source.Location;
  Payload.Rotation = Source.Rotation;
  Payload.AnchorLabel = Source.AnchorLabel;
  return Payload;
}

} // namespace SpawnFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
