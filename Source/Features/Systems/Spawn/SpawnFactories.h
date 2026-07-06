#pragma once

#include "Features/Systems/Spawn/SpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnFactories {

FSpawnState CreateInitialState();
FSpawnPointPayload SpawnPointPayload(const FSpawnPointSource &Source);

} // namespace SpawnFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
