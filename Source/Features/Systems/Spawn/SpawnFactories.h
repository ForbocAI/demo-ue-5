#pragma once

#include "Features/Systems/Spawn/SpawnTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace SpawnFactories {

FSpawnState CreateInitialState();
FSpawnPointPayload SpawnPointPayload(const FSpawnPointSource &Source);

} // namespace SpawnFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
