#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Spawn/SystemsSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnSelectors {

FSpawnPointPayload SelectPlayerSpawn(const FSpawnState &State);

} // namespace SpawnSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
